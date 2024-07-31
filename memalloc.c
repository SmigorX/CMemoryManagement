#include <stdio.h>
#include "memalloc.h"
#include <sys/mman.h>
#include <sys/resource.h>
#include <unistd.h>
#include <math.h>
#include <stdlib.h>

t_heap* heap_start = NULL;

void* allocate_heap_memory(int heap_size) {
    void* heap = (t_heap *)mmap(NULL, heap_size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    
    if (heap == MAP_FAILED) {
        perror("Failed to allocate memory");
        exit(EXIT_FAILURE);
    };

    return heap;
};

size_t determine_heap_size(int allocation_size) {
    if (allocation_size < (SMALL_BLOCK_SIZE - sizeof(t_block))) {
        return SMALL_HEAP_ALLOCATION_SIZE;
    }
    else if (allocation_size < (MEDIUM_BLOCK_SIZE - sizeof(t_block))) {
        return MEDIUM_HEAP_ALLOCATION_SIZE;
    }
    else {
        return (allocation_size + sizeof(t_block) + sizeof(t_heap));
    };

};

void initialize_memory_blocks(t_heap* heap) {
    int heap_size = heap->size; 

    int number_of_blocks = 1;
    int block_size = 0;

    if (heap_size == SMALL_HEAP_ALLOCATION_SIZE) {
        number_of_blocks = (SMALL_HEAP_ALLOCATION_SIZE - sizeof(t_heap))/SMALL_BLOCK_SIZE;
        block_size = SMALL_BLOCK_SIZE;
    }
    else if (heap_size == MEDIUM_HEAP_ALLOCATION_SIZE) {
        number_of_blocks = (MEDIUM_HEAP_ALLOCATION_SIZE - sizeof(t_heap))/MEDIUM_BLOCK_SIZE; 
        block_size = MEDIUM_BLOCK_SIZE;
    }

    t_block* current_block = (t_block*)HEAP_SHIFT(heap);
    for(int i = 0; i < number_of_blocks; i++) {
        current_block->is_free = 1;
        current_block->heap = heap;
        current_block = (t_block*)(&current_block + block_size);
    };

    heap->free_blocks = number_of_blocks;
    heap->block_count = number_of_blocks;
    return;
};

void append_heap(t_heap* new_heap) {
    if (heap_start == NULL) {
        heap_start = new_heap;
    }
    else {
        t_heap* current_heap = heap_start;
        
        while (current_heap->next != NULL) {
            current_heap = current_heap->next;
        }; 

        current_heap->next = new_heap;
        new_heap->prev = current_heap;
    }
};

t_heap* create_heap(int heap_size) {
    t_heap* heap = allocate_heap_memory(heap_size);

    if (heap == NULL) {
        perror("Failed to create heap");
        exit(EXIT_FAILURE);
    };

    heap->prev = NULL;
    heap->next = NULL;
    heap->size = heap_size;

    initialize_memory_blocks(heap);

    append_heap(heap);
    
    return heap;
};

size_t block_size(size_t heap_size) {
    if (heap_size == SMALL_HEAP_ALLOCATION_SIZE) {
        return SMALL_BLOCK_SIZE;
    } 
    else if (heap_size == MEDIUM_HEAP_ALLOCATION_SIZE) {
        return MEDIUM_BLOCK_SIZE;
    } 
}

void* large_heap_malloc(int heap_size) {
    t_heap* heap = create_heap(heap_size);
    if (!heap) {
        return NULL;
    }
    t_block* block = (t_block*)HEAP_SHIFT(heap);
    block->is_free = 0;
    void* data_start = BLOCK_SHIFT(block);
    return data_start;
};

void* memalloc(int allocation_size) {
    if (allocation_size < 1) {
        return NULL; // Invalid allocation size
    }

    size_t heap_size = determine_heap_size(allocation_size);

    // If the allocation size is larger than the maximum block size, create a new heap
    if (allocation_size > (MEDIUM_BLOCK_SIZE - sizeof(t_block))) {
        return large_heap_malloc(heap_size);
    }

    // Initialize heap if not already done
    if (heap_start == NULL) {
        heap_start = create_heap(heap_size);
        if (!heap_start) {
            return NULL; // Heap creation failed
        }
    }

    t_heap* current_heap = heap_start;

    // Search for a free block in the existing heaps
    while (current_heap) {
        if (current_heap->size == heap_size && current_heap->free_blocks > 0) {
            t_block* current_block = (t_block*)HEAP_SHIFT(current_heap);
            size_t size_of_block = block_size(current_heap->size);
            char* heap_end = (char*)current_heap + current_heap->size;

            // Search through blocks in the current heap
            while ((char*)current_block + size_of_block <= heap_end) {
                if (current_block->is_free) {
                    current_block->is_free = 0;
                    current_heap->free_blocks--;
                    void* data_start = BLOCK_SHIFT(current_block);
                    return data_start;
                }
                current_block = (t_block*)((char*)current_block + size_of_block);
            }
        }

        // Move to the next heap if available, otherwise create a new heap
        if (current_heap->next == NULL) {
            t_heap* new_heap = create_heap(heap_size);
            if (!new_heap) {
                return NULL; 
            }

            // Allocate a block from the newly created heap
            t_block* block = (t_block*)HEAP_SHIFT(new_heap);
            block->is_free = 0;
            new_heap->free_blocks--;
            return BLOCK_SHIFT(block);
        } else {
            current_heap = current_heap->next;
        }
    }

    return NULL; 
}
void mefree(void* pointer) {
    if (pointer == NULL) return;

    t_block* block = (t_block*)(pointer - sizeof(t_block));
    block->is_free = 1;
    t_heap* heap = (t_heap*)(block->heap);
    heap->free_blocks++;     

    if (heap->free_blocks == heap->block_count) {
        if (heap->prev != NULL) {
            heap->prev->next = heap->next;
        } else {
            heap_start = heap->next;
        };
 
        if (heap->next != NULL) {
            heap->next->prev = heap->prev;
        };
        
        if (munmap(heap, heap->size) == -1) {
            perror("Failed to release memory");
            exit(EXIT_FAILURE);
        }
    }
};


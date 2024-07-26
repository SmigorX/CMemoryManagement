#include <stdio.h>
#include "memalloc.h"
#include <sys/mman.h>
#include <sys/resource.h>
#include <unistd.h>
#include <math.h>

t_heap* heap_start = NULL;

void* allocate_heap_memory(int heap_size) {
    void* heap = (t_heap *)mmap(NULL, heap_size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    
    if (heap == MAP_FAILED) {
        printf("Failed to allocate memory");
        return NULL;
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
        return (allocation_size + sizeof(t_heap));
    };

};

void initialize_memory_blocks(t_heap* heap) {
    int heap_size = heap->size; //large/medium/small
    
    if (heap_size == SMALL_HEAP_ALLOCATION_SIZE) {
        int number_of_blocks = floor((SMALL_HEAP_ALLOCATION_SIZE - sizeof(t_heap))/SMALL_BLOCK_SIZE);
        
        t_block* current_block = (t_block*)HEAP_SHIFT(heap);
        for(int i = 0; i < 5; i++) {
            current_block->is_free = 1;    
            current_block = (t_block*)((char*)current_block + SMALL_BLOCK_SIZE);
        };

        heap->free_blocks = number_of_blocks;
        heap->block_count = number_of_blocks;
        return;
    } 
    else if (heap_size == MEDIUM_HEAP_ALLOCATION_SIZE) {
        int number_of_blocks = floor((MEDIUM_HEAP_ALLOCATION_SIZE - sizeof(t_heap))/MEDIUM_BLOCK_SIZE);  
 
        t_block* current_block = (t_block*)HEAP_SHIFT(heap);
        for(int i = 0; i < number_of_blocks; i++) {
            current_block->is_free = 1;    
            current_block = (t_block*)((char*)current_block + MEDIUM_BLOCK_SIZE);
        };

        heap->free_blocks = number_of_blocks;
        heap->block_count = number_of_blocks;
        return;
    } 
    else {
        heap->free_blocks = 1;
        heap->block_count = 1;
    };
    
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

t_heap* create_heap(int allocation_size) {
    int heap_size = determine_heap_size(allocation_size); 

    t_heap* heap = allocate_heap_memory(heap_size);

    if (heap == NULL) {
        printf("Failed to create heap");
        return NULL;
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

void* memalloc(int allocation_size) {
    if (allocation_size < 1) {
        return NULL;
    };
    if (allocation_size > (MEDIUM_BLOCK_SIZE - sizeof(t_block))) {
        t_heap* heap = create_heap(allocation_size);
        heap->free_blocks = 0;
        void* data_start = HEAP_SHIFT(heap);
        return data_start;
    }; 
    
    if (heap_start == NULL) {
        create_heap(allocation_size);
    };

    t_heap* current_heap = (t_heap*)heap_start;    
    size_t heap_size = determine_heap_size(allocation_size); 
     
    while (1) {
        if (current_heap->size == heap_size && current_heap->free_blocks > 0) {
            t_block* current_block = (t_block *)HEAP_SHIFT(current_heap); //first block
            while (((char*)current_block + block_size(current_heap->size)) < ((char*)current_heap + heap_size)) {
                if (current_block->is_free == 1) {
                    current_block->is_free = 0;
                    current_heap->free_blocks = current_heap->free_blocks - 1;
                    void* data_start = BLOCK_SHIFT(current_block);
                    return data_start;
                }
                else {
                    current_block = (t_block*)((char*)current_block + block_size(heap_size));
                };
            };
            return NULL;
        } 
        else if (current_heap->next == NULL) {
            create_heap(allocation_size);
        } 
        else {
            current_heap = current_heap->next;
        }
    };
};


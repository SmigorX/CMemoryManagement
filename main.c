#include <stdio.h>
#include "main.h"
#include <sys/mman.h>
#include <sys/resource.h>
#include <unistd.h>

t_heap* heap_start = NULL; 

void* allocate_heap_memory(int heap_size) {
    void* heap = (t_heap *)mmap(NULL, heap_size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    
    if (heap == MAP_FAILED) {
        printf("Failed to allocate memory");
        return NULL;
    };

    return heap;
};

int determine_heap_size(int allocation_size) {
    if (allocation_size < SMALL_BLOCK_SIZE) {
        return SMALL_HEAP_ALLOCATION_SIZE;
    }
    else if (allocation_size < MEDIUM_BLOCK_SIZE) {
        return MEDIUM_HEAP_ALLOCATION_SIZE;
    }
    else {
        return allocation_size;
    };

};

void append_heap(t_heap* new_heap) {
    if (heap_start == NULL) {
        heap_start = new_heap;
    }
    else {
        t_heap* last_heap = heap_start;
        
        while (last_heap->next != NULL) {
            last_heap = last_heap->next;
        }; 

        last_heap->next = new_heap;
        new_heap->prev = last_heap;
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
    heap->total_size = heap_size;
    heap->free_size = heap_size;
    heap->block_count = 0;

    append_heap(heap);

    return heap;
};


void memalloc(int allocation_size) {
    //if (heap_start == NULL) {
    //    t_heap* heap = create_heap(allocation_size);
    //} else {
    //    //make it traverse the heaps
    //    printf("already allocated");
    //};
};

int main() {
    //memalloc(28);
        
    return 0;
};


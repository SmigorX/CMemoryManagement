#include <stdio.h>
#include "main.h"
#include <sys/mman.h>
#include <sys/resource.h>
#include <unistd.h>

void* allocate_heap(int heap_size) {
    //creates new heap
    void* heap = (t_heap *)mmap(NULL, heap_size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    
    if (heap == MAP_FAILED) {
        printf("Failed to allocate memory");
        return NULL;
    };

    return heap;
};

//void memalloc()

void test(){
    t_heap* heap = allocate_heap(SMALL_HEAP_ALLOCATION_SIZE);

    if (heap != NULL) {
        printf("Success");
    };

    heap->prev = NULL;
    heap->next = NULL;
    heap->total_size = SMALL_HEAP_ALLOCATION_SIZE;
    heap->free_size = SMALL_HEAP_ALLOCATION_SIZE;
    heap->block_count = 0;
};

int main() {
    test();

    return 0;
};

//to access the data we should use the pointer to metadata and shift it by offset???

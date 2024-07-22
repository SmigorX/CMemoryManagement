#ifndef EXAMPLE_H
#define EXAMPLE_H

void memalloc(int allocation_size);
void mefree();
void merealloc();
void mecalloc();

typedef struct s_heap {
    struct s_heap *prev;      // Pointer to the previous heap in the list
    struct s_heap *next;      // Pointer to the next heap in the list
    //heap_group group;         // Custom type to group heaps (implementation dependent)
    size_t total_size;        // Total size of the heap
    size_t free_size;         // Amount of free space in the heap
    size_t block_count;       // Number of memory blocks in the heap
} t_heap;

typedef struct s_block {
    struct s_block *prev;    // Pointer to the previous block in the list
    struct s_block *next;    // Pointer to the next block in the list
    size_t data_size;        // Size of the data portion of the block
    int is_free;            // Whether the block is free or allocated
} t_block;

//memory offset of struct size to access data
#define HEAP_SHIFT(start)  ((void *)start + sizeof(t_heap))
#define BLOCK_SHIFT(start)  ((void *)start + sizeof(t_block))

//we split allocations into 3 sizes
#define SMALL_HEAP_ALLOCATION_SIZE (4 * getpagesize())
#define SMALL_BLOCK_SIZE (SMALL_HEAP_ALLOCATION_SIZE / 128)
#define MEDIUM_HEAP_ALLOCATION_SIZE (16 * getpagesize())
#define MEDIUM_BLOCK_SIZE (SMALL_HEAP_ALLOCATION_SIZE / 128)

#endif 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "memalloc.h" // This header should include the declaration of memalloc and any other required declarations

// Function prototypes for testing
void test_small_allocation();
void test_large_allocation();
void test_boundary_allocation();
void test_zero_allocation();
void test_memory_reuse();
void run_tests();

int main() {
    run_tests();
    return 0;
}

void test_small_allocation() {
    void* small_mem = memalloc(10); // Allocate 10 bytes
    if (small_mem == NULL) {
        printf("Small allocation failed\n");
        return;
    }
    memset(small_mem, 1, 10); // Fill memory with 1s
    printf("Small allocation succeeded\n");
}

void test_large_allocation() {
    void* large_mem = memalloc(8000); // Allocate 8000 bytes
    if (large_mem == NULL) {
        printf("Large allocation failed\n");
        return;
    }
    printf("Large allocation succeeded\n");
}

void test_boundary_allocation() {
    void* just_below_block_size = memalloc(SMALL_BLOCK_SIZE - 1);
    if (just_below_block_size == NULL) {
        printf("Allocation just below block size failed\n");
    } else {
        printf("Allocation just below block size succeeded\n");
    }

    void* just_above_block_size = memalloc(SMALL_BLOCK_SIZE + 1);
    if (just_above_block_size == NULL) {
        printf("Allocation just above block size failed\n");
    } else {
        printf("Allocation just above block size succeeded\n");
    }
}

void test_zero_allocation() {
    void* zero_mem = memalloc(0); // Allocate 0 bytes
    if (zero_mem != NULL) {
        printf("Zero-byte allocation should fail, but succeeded\n");
    } else {
        printf("Zero-byte allocation correctly failed\n");
    }
}

void test_memory_reuse() {
    void* mem1 = memalloc(100);
    void* mem2 = memalloc(200);
    memfree(mem1); // Assuming you have a free function
    void* mem3 = memalloc(100); // Should reuse freed memory if possible
    if (mem3 == mem1) {
        printf("Memory reuse test succeeded\n");
    } else {
        printf("Memory reuse test failed\n");
    }
}

void run_tests() {
    printf("Running tests...\n");
    test_small_allocation();
    test_large_allocation();
    test_boundary_allocation();
    test_zero_allocation();
    test_memory_reuse();
}


#include <stdio.h>
#include "memalloc.h"
#include <sys/mman.h>
#include <sys/resource.h>
#include <unistd.h>
#include <math.h>
#include <string.h>
#include "memalloc.c"

void test_small_allocation();
void test_medium_allocation();
void test_large_allocation();
void test_boundary_allocation();
void test_zero_allocation();
void test_memory_reuse();
void test_free();
void run_tests();

int main() {
    run_tests();
    return 0;
}

void test_small_allocation() {
    void* large_mem = memalloc(5); 
    if (large_mem == NULL) {
        printf("FAILED: Couldn't allocate small heap (test_small_alocation)\n");
        return;
    }
    printf("PASSED: Allocated small heap (test_small_allocation)\n");
}

void test_medium_allocation() {
    void* large_mem = memalloc(400); 
    if (large_mem == NULL) {
        printf("FAILED: Couldn't allocate medium heap (test_medium_alocation)\n");
        return;
    }
    printf("PASSED: Allocated medium heap (test_medium_allocation)\n");
}

void test_large_allocation() {
    void* large_mem = memalloc(8000); 
    if (large_mem == NULL) {
        printf("FAILED: Couldn't allocate large heap (test_large_alocation)\n");
        return;
    }
    printf("PASSED: Allocated learge heap (test_large_allocation)\n");
}

void test_boundary_allocation() {
    void* just_below_s_block_size = memalloc(SMALL_BLOCK_SIZE - 1);
    if (just_below_s_block_size == NULL) {
        printf("FAILED: Couldn't allocate just below small block size (test_boundary_allocation)\n");
    } else {
        printf("PASSED: Allocated just below small block size (test_boundary_allocation)\n");
    }

    void* just_above_s_block_size = memalloc(SMALL_BLOCK_SIZE + 1 - sizeof(t_block));
    if (just_above_s_block_size == NULL) {
        printf("FAILED: Couldn't allocate just above small block size (test_boundary_allocation)\n");
    } else {
        printf("PASSED: Allocated just above small block size (test_boundary_allocation)\n");
    }

    void* just_below_m_block_size = memalloc(MEDIUM_BLOCK_SIZE - 1);
    if (just_below_m_block_size == NULL) {
        printf("FAILED: Couldn't allocate just below medium block size (test_boundary_allocation)\n");
    } else {
        printf("PASSED: Allocated just below medium block size (test_boundary_allocation)\n");
    }

    void* just_above_m_block_size = memalloc(MEDIUM_BLOCK_SIZE + 1);
    if (just_above_m_block_size == NULL) {
        printf("FAILED: Couldn't allocate just above medium block size (test_boundary_allocation)\n");
    } else {
        printf("PASSED: Allocated just above medium block size (test_boundary_allocation)\n");
    }
}

void test_zero_allocation() {
    void* zero_mem = memalloc(0); // Allocate 0 bytes
    if (zero_mem != NULL) {
        printf("FAILED: Allocated Zero-bytes (test_zero_allocation)\n");
    } else {
        printf("PASSED: Zero-byte allocation refused (test_zero_allocation)\n");
    }
}

void test_memory_reuse() {
    void* mem1 = memalloc(100);
    void* mem2 = memalloc(200);
    mefree(mem1); 
    void* mem3 = memalloc(100); // Should reuse freed memory if possible
    if (mem3 == mem1) {
        printf("PASSED: Memory reused (test_memory_reuse)\n");
    } else {
        printf("FAILED: Couldn't reuse memory (test_memory_reuse)\n");
    }
}

void test_free() {
    for (int i = 0; i <= 500; i = i + 20) {
        void* mem1 = memalloc(5);
        if (mem1 == NULL) {
            printf("FAILED: Couldn't allocate memory %i (test_free)\n", i);
            return;
        }
        
        mefree(mem1);

        void* mem2 = memalloc(5);
        if (mem2 == NULL) {
            printf("FAILED: Couldn't allocate memory after free %i (test_free)\n", i);
            return;
        }
        
        if (mem2 == mem1) {
        } else {
            printf("FAILED: Different allocation after free, memory not reused %i (test_free)\n", i);
        }
    }
    printf("PASSED: Allocation after free (test_free)\n");
}

void run_tests() {
    printf("Running tests...\n");
    test_small_allocation();
    test_medium_allocation();
    test_large_allocation();
    test_boundary_allocation();
    test_zero_allocation();
    test_memory_reuse();
    test_free();
}


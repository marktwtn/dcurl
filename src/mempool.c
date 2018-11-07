#include "mempool.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

uint32_t block_sizes[MAX_BLOCK_INFO_ENTRY] = {
    BLOCK_SIZE_0,
    BLOCK_SIZE_1,
    BLOCK_SIZE_2,
    BLOCK_SIZE_3,
    BLOCK_SIZE_4,
    BLOCK_SIZE_5,
    BLOCK_SIZE_6,
    BLOCK_SIZE_7,
    BLOCK_SIZE_8,
    BLOCK_SIZE_9,
};

static inline uint64_t rdtsc(void)
{
    unsigned hi, lo;
    __asm__ __volatile__("rdtsc" : "=a"(lo), "=d"(hi));
    return ((uint64_t) lo) | (((uint64_t) hi) << 32);
}

#define TOTAL 1000
static int index = 0;
uint64_t data[TOTAL];
uint64_t length[TOTAL];

bool mempool_init(Pool_t *pool) {
    if (!pool)
        return false;

    pool->pool_size = POOL_SIZE;
    pool->start = malloc(POOL_SIZE);
    if (!pool->start)
        return false;
    pool->end = pool->start + POOL_SIZE;
    pool->current = pool->start;
    for (int idx = 0; idx < MAX_BLOCK_INFO_ENTRY; idx++) {
        pool->block_info_arr[idx].block_size = block_sizes[idx];
        pool->block_info_arr[idx].num_created = 0;
        pool->block_info_arr[idx].num_using = 0;
        pool->block_info_arr[idx].avail_block = NULL;
    }
    return true;
}

// Allocate the block from the memory pool
// Success: return memory address
// Fail: return NULL
void *mempool_alloc(Pool_t *pool, uint32_t size) {
    //uint64_t x0 = rdtsc();
    struct timespec ts, te;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    if (!pool || size == 0)
        return NULL;

    int32_t idx;
    uint8_t *mem;
    Block_t *block_alloc, **block_avail;
    Block_Info_t *block_info;

    idx = find_block_size_index(size);
    if (idx < 0)
        return NULL;

    block_info = &pool->block_info_arr[idx];
    block_avail = &pool->block_info_arr[idx].avail_block;

    if (*block_avail != NULL) {
        block_alloc = *block_avail;
        *block_avail = (*block_avail)->next;
    }
    else {
        if (pool->current + sizeof(Block_t) + block_sizes[idx] > pool->end)
            return NULL;

        block_info->num_created++;
        block_alloc = (Block_t *)pool->current;
        block_alloc->block_size = block_sizes[idx];
        pool->current += sizeof(Block_t) + block_sizes[idx];
        *block_avail = NULL;
    }
    block_alloc->status = ALLOCATED;
    block_alloc->next = NULL;
    block_info->num_using++;
    mem = (uint8_t *)(block_alloc + 1);
    clock_gettime(CLOCK_MONOTONIC, &te);
    //data[index] = rdtsc() - x0;
    data[index] = (te.tv_sec - ts.tv_sec) * 1000000000 + (te.tv_nsec - ts.tv_nsec);
    length[index] = size;
    index++;
    return mem;
}

// Return the allocated block to the memory pool
void mempool_free(Pool_t *pool, void *mem) {
    if (!pool || !mem)
        return;

    int32_t idx;
    Block_t *block_free, **block_avail;
    Block_Info_t *block_info;

    block_free = (Block_t *)(mem - sizeof(Block_t));
    idx = find_block_size_index(block_free->block_size);
    if (idx < 0 || block_free->status != ALLOCATED)
        return;

    block_info = &pool->block_info_arr[idx];
    block_avail = &pool->block_info_arr[idx].avail_block;

    block_free->status = DEALLOCATED;
    block_free->next = *block_avail;
    *block_avail = block_free;
    block_info->num_using--;
    mem = NULL;
}

void mempool_destroy(Pool_t *pool) {
    if (!pool)
        return;

    free(pool->start);
    for (int i = 0; i < index; i++) {
        printf("%ld %ld\n", length[i], data[i]);
    }
}

// Find the index of the corresponding block size to be allocated
// Success: return 0 ~ (MAX_BLOCK_INFO_ENTRY - 1)
// Fail: return -1
int32_t find_block_size_index(uint32_t size) {
    if (size == 0)
        return -1;

    for (int idx = 0; idx < MAX_BLOCK_INFO_ENTRY; idx++) {
        if (size <= block_sizes[idx])
            return idx;
    }
    return -1;
}

#ifndef MEMPOOL_H_
#define MEMPOOL_H_

#include <stdbool.h>
#include <stdint.h>

#define MAX_BLOCK_INFO_ENTRY 10
#define BLOCK_SIZE_0 8
#define BLOCK_SIZE_1 16
#define BLOCK_SIZE_2 28
#define BLOCK_SIZE_3 84
#define BLOCK_SIZE_4 244
#define BLOCK_SIZE_5 732
#define BLOCK_SIZE_6 2596
#define BLOCK_SIZE_7 2676
#define BLOCK_SIZE_8 7780
#define BLOCK_SIZE_9 8020
#define POOL_SIZE 84232

enum {
    ALLOCATED,
    DEALLOCATED
};

typedef struct _block Block_t;
struct _block {
    uint32_t block_size;
    uint32_t status;
    Block_t *next;
};

typedef struct _block_info {
    uint32_t block_size;
    uint32_t num_created;
    uint32_t num_using;
    Block_t *avail_block;
} Block_Info_t;

typedef struct _pool {
    uint32_t pool_size;
    uint8_t *start;
    uint8_t *end;
    uint8_t *current;
    Block_Info_t block_info_arr[MAX_BLOCK_INFO_ENTRY];
} Pool_t;

bool mempool_init(Pool_t *pool);
void *mempool_alloc(Pool_t *pool, uint32_t size);
void mempool_free(Pool_t *pool, void *mem);
void mempool_destroy(Pool_t *pool);
int32_t find_block_size_index(uint32_t size);

Pool_t pool;

#endif

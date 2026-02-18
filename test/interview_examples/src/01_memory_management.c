#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define POOL_CAPACITY 1024U
#define BLOCK_SIZE 64U
#define NUM_BLOCKS (POOL_CAPACITY / BLOCK_SIZE)

typedef struct {
    uint8_t mem[POOL_CAPACITY];
    uint8_t used[NUM_BLOCKS];
} static_pool_t;

static int pool_alloc(static_pool_t *p, uint8_t **out)
{
    size_t i;
    if (!p || !out) {
        return -1;
    }
    for (i = 0U; i < NUM_BLOCKS; ++i) {
        if (p->used[i] == 0U) {
            p->used[i] = 1U;
            *out = &p->mem[i * BLOCK_SIZE];
            return 0;
        }
    }
    return -1;
}

static int pool_free(static_pool_t *p, uint8_t *ptr)
{
    size_t off;
    size_t idx;
    if (!p || !ptr) {
        return -1;
    }
    if (ptr < p->mem || ptr >= p->mem + POOL_CAPACITY) {
        return -1;
    }
    off = (size_t)(ptr - p->mem);
    if (off % BLOCK_SIZE != 0U) {
        return -1;
    }
    idx = off / BLOCK_SIZE;
    p->used[idx] = 0U;
    return 0;
}

int main(void)
{
    static_pool_t pool;
    uint8_t *blocks[NUM_BLOCKS];
    size_t i;

    memset(&pool, 0, sizeof(pool));

    for (i = 0U; i < NUM_BLOCKS; ++i) {
        if (pool_alloc(&pool, &blocks[i]) != 0) {
            printf("Pool allocation failed at block %zu\n", i);
            return 1;
        }
    }

    if (pool_alloc(&pool, &blocks[0]) != 0) {
        printf("Pool full as expected (deterministic failure)\n");
    }

    for (i = 0U; i < NUM_BLOCKS; i += 2U) {
        (void)pool_free(&pool, blocks[i]);
    }

    for (i = 0U; i < NUM_BLOCKS / 2U; ++i) {
        uint8_t *tmp = NULL;
        if (pool_alloc(&pool, &tmp) != 0) {
            printf("Unexpected pool alloc failure\n");
            return 2;
        }
    }

    printf("Static pool demo complete. Contrast: malloc/free can fragment over time.\n");
    return 0;
}

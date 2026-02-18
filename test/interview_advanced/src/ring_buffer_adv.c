#include "ring_buffer_adv.h"

int rb_adv_init(rb_adv_t *rb, float *storage, size_t capacity)
{
    if (!rb || !storage || capacity == 0U) {
        return -1;
    }

    rb->data = storage;
    rb->capacity = capacity;
    rb->head = 0U;
    rb->tail = 0U;
    rb->count = 0U;
    rb->overflow_count = 0UL;
    rb->underflow_count = 0UL;
    return 0;
}

int rb_adv_push(rb_adv_t *rb, float v)
{
    if (!rb || !rb->data) {
        return -1;
    }

    if (rb->count == rb->capacity) {
        rb->overflow_count++;
        return -1;
    }

    rb->data[rb->head] = v;
    rb->head = (rb->head + 1U) % rb->capacity;
    rb->count++;
    return 0;
}

int rb_adv_pop(rb_adv_t *rb, float *out)
{
    if (!rb || !rb->data || !out) {
        return -1;
    }

    if (rb->count == 0U) {
        rb->underflow_count++;
        return -1;
    }

    *out = rb->data[rb->tail];
    rb->tail = (rb->tail + 1U) % rb->capacity;
    rb->count--;
    return 0;
}

size_t rb_adv_size(const rb_adv_t *rb)
{
    return rb ? rb->count : 0U;
}

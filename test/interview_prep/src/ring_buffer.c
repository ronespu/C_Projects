#include "ring_buffer.h"

int rb_init(ring_buffer_t *rb, float *storage, size_t capacity)
{
    if (!rb || !storage || capacity == 0U) {
        return -1;
    }

    rb->data = storage;
    rb->capacity = capacity;
    rb->count = 0U;
    rb->head = 0U;
    rb->tail = 0U;
    return 0;
}

int rb_push(ring_buffer_t *rb, float sample)
{
    if (!rb || !rb->data) {
        return -1;
    }

    rb->data[rb->head] = sample;
    rb->head = (rb->head + 1U) % rb->capacity;

    if (rb->count == rb->capacity) {
        rb->tail = (rb->tail + 1U) % rb->capacity;
    } else {
        rb->count++;
    }
    return 0;
}

int rb_pop(ring_buffer_t *rb, float *sample)
{
    if (!rb || !sample || rb->count == 0U) {
        return -1;
    }

    *sample = rb->data[rb->tail];
    rb->tail = (rb->tail + 1U) % rb->capacity;
    rb->count--;
    return 0;
}

size_t rb_size(const ring_buffer_t *rb)
{
    return rb ? rb->count : 0U;
}

size_t rb_capacity(const ring_buffer_t *rb)
{
    return rb ? rb->capacity : 0U;
}

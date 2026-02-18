#ifndef RING_BUFFER_H
#define RING_BUFFER_H

#include <stddef.h>

typedef struct {
    float *data;
    size_t capacity;
    size_t count;
    size_t head;
    size_t tail;
} ring_buffer_t;

int rb_init(ring_buffer_t *rb, float *storage, size_t capacity);
int rb_push(ring_buffer_t *rb, float sample);
int rb_pop(ring_buffer_t *rb, float *sample);
size_t rb_size(const ring_buffer_t *rb);
size_t rb_capacity(const ring_buffer_t *rb);

#endif

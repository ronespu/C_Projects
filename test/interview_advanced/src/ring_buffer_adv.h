#ifndef RING_BUFFER_ADV_H
#define RING_BUFFER_ADV_H

#include <stddef.h>

typedef struct {
    float *data;
    size_t capacity;
    size_t head;
    size_t tail;
    size_t count;
    unsigned long overflow_count;
    unsigned long underflow_count;
} rb_adv_t;

int rb_adv_init(rb_adv_t *rb, float *storage, size_t capacity);
int rb_adv_push(rb_adv_t *rb, float v);
int rb_adv_pop(rb_adv_t *rb, float *out);
size_t rb_adv_size(const rb_adv_t *rb);

#endif

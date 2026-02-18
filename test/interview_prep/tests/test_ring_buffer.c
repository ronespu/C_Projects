#include "ring_buffer.h"

#include <assert.h>
#include <math.h>

int main(void)
{
    float storage[4];
    ring_buffer_t rb;
    float out = 0.0f;

    assert(rb_init(&rb, storage, 4) == 0);
    assert(rb_size(&rb) == 0);

    assert(rb_push(&rb, 1.0f) == 0);
    assert(rb_push(&rb, 2.0f) == 0);
    assert(rb_push(&rb, 3.0f) == 0);
    assert(rb_push(&rb, 4.0f) == 0);
    assert(rb_size(&rb) == 4);

    assert(rb_push(&rb, 5.0f) == 0);
    assert(rb_size(&rb) == 4);

    assert(rb_pop(&rb, &out) == 0);
    assert(fabsf(out - 2.0f) < 1e-6f);
    assert(rb_pop(&rb, &out) == 0);
    assert(fabsf(out - 3.0f) < 1e-6f);
    assert(rb_pop(&rb, &out) == 0);
    assert(fabsf(out - 4.0f) < 1e-6f);
    assert(rb_pop(&rb, &out) == 0);
    assert(fabsf(out - 5.0f) < 1e-6f);
    assert(rb_pop(&rb, &out) == -1);

    return 0;
}

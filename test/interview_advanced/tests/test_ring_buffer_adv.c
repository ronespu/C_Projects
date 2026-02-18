#include <assert.h>

#include "ring_buffer_adv.h"

int main(void)
{
    float mem[4] = {0};
    rb_adv_t rb;
    float out = 0.0f;

    assert(rb_adv_init(&rb, mem, 4) == 0);
    assert(rb_adv_init(0, mem, 4) == -1);
    assert(rb_adv_init(&rb, 0, 4) == -1);
    assert(rb_adv_init(&rb, mem, 0) == -1);

    assert(rb_adv_pop(&rb, &out) == -1);
    assert(rb.underflow_count == 1UL);

    assert(rb_adv_push(&rb, 1.0f) == 0);
    assert(rb_adv_push(&rb, 2.0f) == 0);
    assert(rb_adv_push(&rb, 3.0f) == 0);
    assert(rb_adv_push(&rb, 4.0f) == 0);
    assert(rb_adv_size(&rb) == 4U);
    assert(rb_adv_push(&rb, 5.0f) == -1);
    assert(rb.overflow_count == 1UL);

    assert(rb_adv_pop(&rb, &out) == 0);
    assert(out == 1.0f);
    assert(rb_adv_pop(&rb, &out) == 0);
    assert(out == 2.0f);
    return 0;
}

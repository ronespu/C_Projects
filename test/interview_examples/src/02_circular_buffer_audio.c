#include <math.h>
#include <stdio.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define RB_CAP 16

typedef struct {
    float data[RB_CAP];
    int head;
    int tail;
    int count;
} rb_t;

static void rb_init(rb_t *rb)
{
    rb->head = 0;
    rb->tail = 0;
    rb->count = 0;
}

static void rb_push_overwrite(rb_t *rb, float s)
{
    rb->data[rb->head] = s;
    rb->head = (rb->head + 1) % RB_CAP;
    if (rb->count == RB_CAP) {
        rb->tail = (rb->tail + 1) % RB_CAP;
    } else {
        rb->count++;
    }
}

static int rb_pop(rb_t *rb, float *s)
{
    if (rb->count == 0) {
        return -1;
    }
    *s = rb->data[rb->tail];
    rb->tail = (rb->tail + 1) % RB_CAP;
    rb->count--;
    return 0;
}

int main(void)
{
    rb_t rb;
    int i;
    rb_init(&rb);

    for (i = 0; i < 24; ++i) {
        float t = (float)i / 16000.0f;
        float sample = sinf(2.0f * (float)M_PI * 1000.0f * t);
        rb_push_overwrite(&rb, sample);
    }

    printf("Ring buffer contains last %d samples:\n", rb.count);
    while (rb.count > 0) {
        float x;
        (void)rb_pop(&rb, &x);
        printf("%.4f\n", x);
    }
    return 0;
}

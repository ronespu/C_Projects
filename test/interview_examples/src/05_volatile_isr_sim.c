#include <signal.h>
#include <stdio.h>
#include <unistd.h>

static volatile sig_atomic_t g_flag = 0;

static void isr_handler(int signo)
{
    (void)signo;
    g_flag = 1;
}

int main(void)
{
    int loops = 0;

    signal(SIGALRM, isr_handler);
    alarm(1);

    printf("Waiting simulated ISR signal...\n");
    while (!g_flag) {
        loops++;
    }

    printf("Flag observed from main loop after ISR. loops=%d\n", loops);
    printf("Interview point: shared ISR/main variables should be volatile (+sync strategy if complex).\n");
    return 0;
}

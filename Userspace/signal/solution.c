#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

void init(void);

int main(void)
{
    init();
    do {
        pause(); // sleep between signal handler invocations
    } while (1);
    return 0;
}

void sig_hdlr(int signal);

void init(void)
{
    signal(SIGUSR1, sig_hdlr);
    signal(SIGUSR2, sig_hdlr);
    signal(SIGTERM, sig_hdlr);
}

void sig_hdlr(int signal)
{
    static int count_1 = 0;
    static int count_2 = 0;

    switch (signal) {
    case SIGUSR1:
        count_1++;
        break;
    case SIGUSR2:
        count_2++;
        break;
    case SIGTERM:
        printf("%d %d\n", count_1, count_2);
        fflush(stdout);
        _exit(0);
    default:
        break;
    }
}


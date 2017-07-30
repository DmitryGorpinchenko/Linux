#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

int daemonize(void);

int main(void)
{
    if (daemonize() < 0) {
        return 1;
    }
    pause(); // simulate daemon functionality
    return 0;
}

void sig_hdlr(int sig);

int daemonize(void)
{
    int pid;
    if ((pid = fork()) == 0) {
        //printf("%d\n", getpid());
        if ((chdir("/") < 0)
        ||  (setsid() < 0)
        ||  (close(STDIN_FILENO) < 0)
        ||  (close(STDOUT_FILENO) < 0)
        ||  (close(STDERR_FILENO) < 0)) {
            return -1;
        }
        signal(SIGURG, sig_hdlr);
    } else if (pid > 0) {
        exit(0);
    } else {
        return -1;
    }
    return 0;
}

void sig_hdlr(int sig)
{
    if (sig == SIGURG) {
        exit(0);
    }
}

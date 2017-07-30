#include <unistd.h>
#include <stdio.h>

int get_ppid(int pid);

int main(int argc, char **argv)
{
    if (argc < 2) {
        return -1;
    }
    int pid;
    if (sscanf(argv[1], "%d", &pid) != 1) {
        return -1;
    }
    printf("%d\n", pid);
    do {
        if ((pid = get_ppid(pid)) < 0) {
            return -1;
        } else {
            printf("%d\n", pid);
        }
    } while (pid != 1);
    return 0;
}

#define MAX_FNAME_LEN 256

int get_ppid(int pid)
{
    int ppid;
    char stat_str[MAX_FNAME_LEN];
    FILE *stat;

    if (sprintf(stat_str, "/proc/%d/stat", pid) < 0) {
        return -1;
    }
    if ((stat = fopen(stat_str, "r")) == NULL) {
        return -1;
    }
    fscanf(stat, "%*d %*s %*c %d", &ppid);
    fclose(stat);
    return ppid;
}


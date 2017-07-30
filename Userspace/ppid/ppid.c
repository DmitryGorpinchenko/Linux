#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_FNAME_LEN 256

int get_ppid(int pid);

int main()
{
    printf("%d\n", get_ppid(getpid()));
    return 0;
}

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


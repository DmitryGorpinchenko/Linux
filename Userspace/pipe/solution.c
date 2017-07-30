#define  _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

int sym_count(FILE * stream, char sym);

int main(int argc, char **argv)
{
    char *comm;
    if (asprintf(&comm, "%s %s", argv[1], argv[2]) < 0) {
        return -1;
    }
    FILE *pipe = popen(comm, "r");
    if (pipe == NULL) {
        return -1;
    }
    printf("%d\n", sym_count(pipe, '0'));
    pclose(pipe);
    free(comm);
    return 0;
}

int sym_count(FILE * stream, char sym)
{
    int count = 0;
    char c;
    while (fread(&c, sizeof(char), 1, stream)) {
        if (c == sym) {
            count++;
        }
    }
    return count;
}


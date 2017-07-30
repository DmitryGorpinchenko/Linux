#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <ctype.h>

int proc_num(const char *proc_name);

int main()
{
    printf("%d\n", proc_num("genenv"));
    return 0;
}

#define MAX_FNAME_LEN 256

int str_to_unum(const char *str);

int proc_num(const char *name_to_search)
{
    char comm[MAX_FNAME_LEN];
    char proc_name[MAX_FNAME_LEN];
    int pid;
    int count = 0;
    DIR *proc = opendir("/proc");
    FILE *pcomm;
    struct dirent *entry;
    while ((entry = readdir(proc)) != NULL) {
        if (!(entry->d_type & DT_DIR)) {
            continue;
        }
        if ((pid = str_to_unum(entry->d_name)) < 0) {
            continue;
        }
        if (sprintf(comm, "/proc/%d/comm", pid) < 0) {
            continue;
        }
        if ((pcomm = fopen(comm, "r")) == NULL) {
            continue;
        }
        if (fscanf(pcomm, "%s", proc_name) == EOF) {
            continue;
        }
        if (strcmp(proc_name, name_to_search) == 0) {
            count++;
        }
        fclose(pcomm);
    }
    closedir(proc);
    return count;
}

int str_to_unum(const char *str)
{
    int num = 0;
    while (*str) {
        if (isdigit(*str)) {
            num = 10 * num + (*str++ - '0');
        } else {
            return -1;
        }
    }
    return num;
}


#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <ctype.h>

int descendants_num(int root);

int main(int argc, char **argv)
{
    if (argc < 2) {
        return -1;
    }
    int root;
    if (sscanf(argv[1], "%d", &root) != 1) {
        return -1;
    }
    printf("%d\n", descendants_num(root));
    return 0;
}

#define MAX_FNAME_LEN 256
#define MAX_PID_LIMIT (1 << 22)

typedef enum { UNSURE, YES, NO } fuzzy;

int has_path(int pid, int root);
int get_ppid(int pid);
int str_to_num(const char *str);
int boolean(fuzzy val);
int certain(fuzzy val);

int descendants_num(int root)
{
    int count = 0;
    DIR *proc = opendir("/proc");
    struct dirent *entry;
    while ((entry = readdir(proc)) != NULL) {
        if (entry->d_type & DT_DIR) {
            if (has_path(str_to_num(entry->d_name), root)) {
                count++;
            }
        }
    }
    closedir(proc);
    return count;
}

int has_path(int pid, int root)
{
    static fuzzy fuzzy_cache[MAX_PID_LIMIT] = { UNSURE }; // works because UNSURE == 0

    if (certain(fuzzy_cache[pid])) {
        return boolean(fuzzy_cache[pid]);
    }
    if (pid <= 1) {
        return 0;
    }
    if (pid == root) {
        return 1;
    }
    return boolean(fuzzy_cache[pid] = (has_path(get_ppid(pid), root) ? YES : NO));
}

int boolean(fuzzy val)
{
    return val == YES ? 1 : 0;
}

int certain(fuzzy val)
{
    return val != UNSURE;
}

int get_ppid(int pid)
{
    int ppid;
    char stat_str[MAX_FNAME_LEN] = { 0 };
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

int str_to_num(const char *str)
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


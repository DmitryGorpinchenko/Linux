#include <sys/select.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>

#define FD_NUM   2
#define BUF_SIZE 4096

const char *pipes[] = { "in1", "in2" };

int main()
{
    char nums[BUF_SIZE];
    int fds[FD_NUM];
    int closed[FD_NUM];
    int i, j, bytes, sum = 0;
    fd_set read_set;
	
    for (i = 0; i < FD_NUM; i++) {
        if ((fds[i] = open(pipes[i], O_RDONLY)) < 0) {
            return 1;
        }
    }
    memset(closed, 0, sizeof(int) * FD_NUM);
	
    do {
        int closed_num = 0;
        FD_ZERO(&read_set);
        for (i = 0; i < FD_NUM; i++) {
            if (closed[i]) {
                closed_num++;
            } else {
                FD_SET(fds[i], &read_set);
            }
        }
        if (closed_num == FD_NUM || select(fds[FD_NUM - 1] + 1, &read_set, NULL, NULL, NULL) < 0) {
            break;
        }
        for (i = 0; i < FD_NUM; i++) {
            if (FD_ISSET(fds[i], &read_set)) {
                if ((bytes = read(fds[i], nums, BUF_SIZE)) > 0) {
                    for (j = 0; j < bytes; j++) {
                        sum += (nums[j] - '0');
                    }
                } else if (bytes == 0) {
                    close(fds[i]);
                    closed[i] = 1;
                }
            }
        }
    } while (1);

    printf("%d\n", sum);
    return 0;
}


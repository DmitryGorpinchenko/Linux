#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

int init_tcp_socket(const char *addr, short port);
void serve(int cs);

int main(int argc, char **argv)
{
    if (argc < 2) {
        return -1;
    }
    short port;
    if (sscanf(argv[1], "%hd", &port) != 1) {
        return -1;
    }
    int s;
    if ((s = init_tcp_socket("127.0.0.1", port)) < 0) {
        return -1;
    }
    if (listen(s, SOMAXCONN) < 0) {
        return -1;
    }
    serve(accept(s, NULL, NULL));
    close(s);
    return 0;
}

#define MAX_SYM_NUM 256

char *sort_symbols(char *str);

void serve(int cs)
{
    if (cs < 0) {
        return;
    }
    int bytes;
    char buf[BUFSIZ] = { 0 };
    do {
        if ((bytes = read(cs, buf, BUFSIZ)) <= 0
        ||  (strncmp("OFF", buf, bytes) == 0)) {
            break;
        }
        write(cs, sort_symbols(buf), bytes);
    } while (1);
    close(cs);
}

char *sort_symbols(char *str)
{
    int i, j;
    char *ptr = str;
    int counts[MAX_SYM_NUM] = { 0 };
    while (*ptr) {
        counts[*ptr++]++;
    }
    ptr = str;
    for (i = MAX_SYM_NUM - 1; i >= 0; i--) { // sort in reverse ASCII-code order
        for (j = 0; j < counts[i]; j++) {
            *ptr++ = i;
        }
    }
    return str;
}

int init_tcp_socket(const char *addr, short port)
{
    int s;
    int family = AF_INET;
    if ((s = socket(family, SOCK_STREAM, 0)) < 0) {
        return -1;
    }
    struct sockaddr_in in_addr;
    memset(&in_addr, 0, sizeof(struct sockaddr_in));
    in_addr.sin_family = family;
    in_addr.sin_port = htons(port);
    if (inet_pton(family, addr, &in_addr.sin_addr) <= 0) {
        return -1;
    }
    if (bind(s, (struct sockaddr *)&in_addr, sizeof(struct sockaddr_in)) < 0) {
        return -1;
    }
    return s;
}


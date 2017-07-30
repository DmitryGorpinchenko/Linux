#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

int init_udp_socket(const char *addr, short port);
void log_requests(int s);

int main(int argc, char **argv)
{
    if (argc < 2) {
        return -1;
    }
    short port;
    if (sscanf(argv[1], "%hu", &port) != 1) {
        return -1;
    }
    int s;
    if ((s = init_udp_socket("127.0.0.1", port)) < 0) {
        return -1;
    }
    log_requests(s);
    close(s);
    return 0;
}

#define DGRAM_MAX_LEN 5120

int init_udp_socket(const char *addr, short port)
{
    int s;
    int family = AF_INET;
    if ((s = socket(family, SOCK_DGRAM, 0)) < 0) {
        return -1;
    }
    struct sockaddr_in in_addr;
    memset(&in_addr, 0, sizeof(struct sockaddr_in));
    in_addr.sin_family = family;
    in_addr.sin_port = htons(port);
    if (inet_pton(family, addr, &in_addr.sin_addr) <= 0) {
        return -1;
    }
    if (bind(s, (struct sockaddr *) &in_addr, sizeof(struct sockaddr_in)) < 0) {
        return -1;
    }
    return s;
}

void log_requests(int s)
{
    int bytes;
    char req[DGRAM_MAX_LEN + 1];
    do {
        if ((bytes = read(s, req, DGRAM_MAX_LEN)) < 0) {
            break;
        }
        req[bytes] = '\0';
        if (strcmp(req, "OFF\n") == 0) {
            break;
        }
        printf("%s\n", req);
    } while (1);
}


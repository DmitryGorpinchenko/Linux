#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <arpa/inet.h>

int main(int argc, char **argv)
{
    if (argc < 2) {
        return -1;
    }
    struct hostent *info;
    if ((info = gethostbyname(argv[1])) == NULL) {
        return -1;
    }
	
    int size = (info->h_addrtype == AF_INET) ? INET_ADDRSTRLEN : INET6_ADDRSTRLEN;
    char *dst = malloc(size);

    int i = 0;
    while (info->h_addr_list[i]) {
        if (inet_ntop(info->h_addrtype, info->h_addr_list[i++], dst, size)) {
            printf("%s\n", dst);
        }
    }
    free(dst);
    return 0;
}


#include <stdio.h>
#include <unistd.h>

int
main(int argc, char **argv)
{
	int n;
	sscanf(argv[1], "%d", &n);
	for (int i = 0; i < n; i++) {
		printf("%c", '0');
	}
	fflush(stdout);
	sleep(5);
	for (int i = 0; i < n; i++) {
		printf("%c", '0');
	}
	return 0;
}

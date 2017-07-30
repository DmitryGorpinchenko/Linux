#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

int main(int argc, char **argv)
{
	int q_flag = 0, i_flag = 0, v_flag = 0, optid, optchar;

	struct option opts[] = {
		{"query", required_argument, &q_flag, 1},
		{"longinformationrequest", no_argument, &i_flag, 1},
		{"version", no_argument, &v_flag, 1}
	};

	while ((optchar = getopt_long(argc, argv, "q:iv", opts, &optid)) != -1)
		if (optchar == '?') {
			printf("-\n");
			exit(1);
		}

	printf("+\n");
	return 0;
}

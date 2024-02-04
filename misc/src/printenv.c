/* See LICENSE file for copyright and license details. */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "util.h"


extern char **environ;

static void
usage(void)
{
	die(1, "usage: printenv [name...]");
}

int
main(int argc, char *argv[])
{
	int opt;
	int ret = 0;
	char *var;


	while ((opt = getopt(argc, argv, "")) != -1) {
		switch (opt) {
		default:
			usage();
			break;
		}
	}
	argc -= optind;
	argv += optind;

	if (argc < 1) {
		for (char **e = environ; *e; ++e)
			puts(*e);
	} else {
		for (; *argv; ++argv) {
			if ((var = getenv(*argv)) != NULL)
				puts(var);
			else
				ret = 1;
		}
	}

	return ret;
}

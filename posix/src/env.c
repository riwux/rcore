/* See LICENSE file for copyright and license details. */
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "util.h"


extern char **environ;

static void
usage(void)
{
	die(1, "usage: env [-i] [name=value]... [utility [arg...]]");
}

int
main(int argc, char *argv[])
{
	int opt;

	while ((opt = getopt(argc, argv, "i")) != -1) {
		switch (opt) {
		case 'i':
			/* Trick putenv(3) into allocating a new environment. */
			*environ = NULL;
			break;
		default:
			usage();
			break;
		}
	}
	argc -= optind;
	argv += optind;

	/* Historically, '-' has the same effect as '-i'. */
	if (*argv && !strcmp(*argv, "-")) {
		*environ = NULL;
		++argv;
	}

	/* Take care of the variables. */
	for (; *argv && strchr(*argv, '='); ++argv) {
		if (putenv(*argv))
			die(1, "putenv:");
	}

	/* No utility was specified. */
	if (!*argv) {
		for (char **e = environ; *e; ++e)
			puts(*e);
	} else {
		execvp(*argv, argv);
		die((errno == ENOENT) ? 127 : 126, "execvp: '%s':", *argv);
	}
}

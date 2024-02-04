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

static inline _Bool
is_valid_arg(char *arg)
{
	_Bool ret = 0;

	if (arg[0] != '=' && strchr(arg, '='))
		ret = 1;

	return ret;
}

int
main(int argc, char *argv[])
{
	int opt;

	while ((opt = getopt(argc, argv, "i")) != -1) {
		switch (opt) {
		case 'i':
			*environ = NULL;
			break;
		default:
			usage();
			break;
		}
	}
	argc -= optind;
	argv += optind;

	/* Take care of the variables. */
	for (; *argv && is_valid_arg(*argv); ++argv, --argc) {
		if (putenv(*argv))
			die(1, "putenv:");
	}

	/* No utility was specified. */
	if (argc == 0) {
		for (char **e = environ; *e; ++e)
			puts(*e);
	} else {
		execvp(*argv, argv);
		die((errno == ENOENT) ? 127 : 126, "execvp: '%s':", *argv);
	}
}

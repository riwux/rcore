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
	_Bool iflag = 0;
	int opt;
	char **envp;

	while ((opt = getopt(argc, argv, "i")) != -1) {
		switch (opt) {
		case 'i':
			envp  = environ;
			iflag = 1;
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
		if (iflag) {
			*envp = *argv;
			++envp;
		} else {
			if (putenv(*argv))
				die(1, "putenv:");
		}
	}

	/* Make sure to "cut off" the old env. */
	if (iflag)
		*envp = NULL;

	/* No utility was specified. */
	if (argc == 0) {
		for (char **e = environ; *e; ++e)
			puts(*e);
	} else {
		execvp(*argv, argv);
		die((errno == ENOENT) ? 127 : 126, "execvp: '%s':", *argv);
	}

	return 0;
}

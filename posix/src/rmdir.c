/* See LICENSE file for copyright and license details. */
#include <errno.h>
#include <libgen.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "util.h"

static void
usage(void)
{
	die(1, "usage: rmdir [-p] dir...");
}

static bool
rmdir_path(char *path)
{
	bool ret = false;

	do {
		if (rmdir(path)) {
			warn("rmdir: cannot remove '%s':", path);
			ret = 1;
		}
		path = dirname(path);
	} while (strcmp(path, "/") && strcmp(path, "."));

	return ret;
}

int
main(int argc, char *argv[])
{
	bool pflag = false;
	int opt;
	int ret = 0;

	while ((opt = getopt(argc, argv, "p")) != -1) {
		switch (opt) {
		case 'p':
			pflag = true;
			break;
		default:
			usage();
			break;
		}
	}
	argc -= optind;
	argv += optind;

	if (argc < 1)
		usage();

	for (; *argv; ++argv) {
		if (pflag) {
			if (rmdir_path(*argv))
				ret = 1;
		} else if (rmdir(*argv)) {
			warn("rmdir: cannot remove '%s':", *argv);
			ret = 1;
		}
	}

	return ret;
}

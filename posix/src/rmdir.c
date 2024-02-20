/* See LICENSE file for copyright and license details. */
#include <errno.h>
#include <libgen.h>
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

static _Bool
rmdir_path(char *path)
{
	_Bool ret = 0;

	do {
		if (rmdir(path)) {
			eprintf("rmdir: cannot remove '%s': %s\n", path, strerror(errno));
			ret = 1;
		}
		path = dirname(path);
	} while (strcmp(path, "/") && strcmp(path, "."));

	return ret;
}

int
main(int argc, char *argv[])
{
	_Bool pflag = 0;
	int opt, ret;

	while ((opt = getopt(argc, argv, "p")) != -1) {
		switch (opt) {
		case 'p':
			pflag = 1;
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

	do {
		if (pflag) {
			ret = rmdir_path(*argv);
		} else if (rmdir(*argv)) {
			eprintf("rmdir: cannot remove '%s': %s\n", *argv, strerror(errno));
			ret = 1;
		}
	} while (*++argv);

	return ret;
}

/* See LICENSE file for copyright and license details. */
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

static char *
get_pnc(char *path)
{
	static _Bool first = 1;
	char *p;

	/* dirname(3) would skip the last path name component */
	if (first) {
		first = 0;
		return path;
	}

	p = dirname(path);
	if (!strcmp(p, "/") || !strcmp(p, "."))
		p = NULL;

	return p;
}

static _Bool
rmdir_path(char *path)
{
	_Bool ret = 0;
	char *p;

	while ((p = get_pnc(path))) {
		if (rmdir(p)) {
			eprintf("rmdir: '%s': ", p);
			perror(NULL);
			ret = 1;
		}
	}

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
			eprintf("rmdir: '%s': ", *argv);
			perror(NULL);
			ret = 1;
		}
	} while (*++argv);

	return ret;
}

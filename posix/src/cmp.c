/* See LICENSE file for copyright and license details. */
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "util.h"

static void
usage(void)
{
	die(2, "usage: cmp [-l|-s] file1 file2");
}

int
main(int argc, char *argv[])
{
	bool lflag = false;
	bool sflag = false;
	bool eof   = false;
	int ret    = 0;
	int opt;
	size_t n, ln;
	int c[2];
	char *fn[2];
	FILE *fp[2];

	while ((opt = getopt(argc, argv, "ls")) != -1) {
		switch (opt) {
		case 'l':
			lflag = true;
			sflag = false;
			break;
		case 's':
			lflag = false;
			sflag = true;
			break;
		default:
			usage();
			break;
		}
	}
	argc -= optind;
	argv += optind;

	if (argc != 2)
		usage();

	for (int i = 0; i < 2; ++i) {
		fn[i] = argv[i];
		fp[i] = (!strcmp(fn[i], "-")) ? stdin : fopen(fn[i], "r");
		if (!fp[i])
			die(2, "fopen: '%s':", fn[i]);
	}

	/* Is the file compared to itself? */
	if (!strcmp(fn[0], fn[1]))
		return 0;

	for (n = ln = 1;; ++n) {
		c[0] = fgetc(fp[0]);
		c[1] = fgetc(fp[1]);

		if (feof(fp[0]) || feof(fp[1])) {
			eof = true;
			break;
		}
		if (c[0] == '\n')
			++ln;
		if (c[0] != c[1]) {
			ret = 1;
			if (sflag) {
				break;
			} else if (lflag) {
				printf("%zu %o %o\n", n, c[0], c[1]);
			} else {
				printf("%s %s differ: byte %zu, line %zu\n", \
				        fn[0], fn[1], n, ln);
				break;
			}
		}
	}
	if (eof) {
		eprintf("cmp: EOF on %s after byte %zu\n", \
		         fn[1], n-1);
	}
	return ret;
}

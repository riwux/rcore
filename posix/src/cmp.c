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

static bool
compare_files(char *fn[], FILE *fp[], bool lflag, bool sflag)
{
	size_t n  = 1;
	size_t ln = 1;
	int c[2]  = {-2, -2};

	for (int i = 0; ; i=!i, n+=!i) {
		c[i] = fgetc(fp[i]);
		if (c[i] == EOF) {
			if (c[!i] != EOF && fgetc(fp[!i]) != EOF)
				fn[2] = fn[i];
			break;
		}
		if (c[!i] == -2)
			continue;
		if (c[i] == '\n' && c[!i] == '\n')
			++ln;
		if (c[i] != c[!i]) {
			if (sflag) {
				break;
			} else if (lflag) {
				printf("%zu %o %o\n", n, c[!i], c[i]);
			} else {
				printf("%s %s differ: byte %zu, line %zu\n", \
				        fn[!i], fn[i], n, ln);
				break;
			}
		}
		c[i] = c[!i] = -2;
	}
	if (fn[2]) {
		eprintf("cmp: EOF on %s after byte %zu\n", \
		         fn[2], n-1);
	}

	return (c[0] != c[1]);
}

int
main(int argc, char *argv[])
{
	bool lflag = false;
	bool sflag = false;
	int opt;
	char *fn[3] = {NULL, NULL, NULL};
	FILE *fp[2] = {NULL, NULL};

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

	return compare_files(fn, fp, lflag, sflag);
}

/* See LICENSE file for copyright and license details. */
#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "util.h"

static void
usage(void)
{
	die(1, "usage: head [-n num] [file...]");
}

static int
head(FILE *fp, char *file, int64_t num)
{
	size_t len = 0;
	char *line = NULL;

	for (int64_t i = 0; i < num && getline(&line, &len, fp) != -1; ++i)
		fputs(line, stdout);
	free(line);

	if (ferror(fp)) {
		eprintf("getline: cannot read '%s': %s\n", file, strerror(errno));
		return -1;
	}
	return 0;
}

int
main(int argc, char *argv[])
{
	int opt;
	int ret     = 0;
	int64_t num = 10;
	FILE *fp;

	while ((opt = getopt(argc, argv, "n:")) != -1) {
		switch (opt) {
		case 'n':
			num = to_num(optarg, DEC);
			if (num < 0)
				die(1, "head: '%lld': argument has to be positive", num);
			break;
		default:
			usage();
			break;
		}
	}
	argc -= optind;
	argv += optind;

	for (int i = 0; i < argc; ++i) {
		if (!strcmp(argv[i], "-")) {
			fp = stdin;
		} else {
			if (!(fp = fopen(argv[i], "r"))) {
				eprintf("fopen: cannot open '%s' for reading: %s\n", argv[i], \
				    strerror(errno));
				ret = 1;
				continue;
			}
		}
		if (argc > 1) {
			if (i > 1)
				fputc('\n', stdout);
			printf("==> %s <==\n", (fp == stdin ? "stdin" : argv[i]));
		}
		if (head(fp, argv[i], num))
			ret = 1;
		if (fp != stdin)
			fclose(fp);
	}
	return ret;
}

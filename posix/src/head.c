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
	die(1, "usage: head [-n num] [-num] [file...]");
}

static int
head(char const *file, FILE *fp, int64_t num)
{
	ssize_t n;
	size_t len = 0;
	char *line = NULL;

	for (int64_t i = 0; i < num && (n = getline(&line, &len, fp)) != -1; ++i)
		fwrite(line, n, 1, stdout);
	free(line);

	if (ferror(fp)) {
		warn("head: getline '%s':", file);
		return -1;
	}

	return 0;
}

int
main(int argc, char *argv[])
{
	int opt;
	int bsdopt  = 0;
	int ret     = 0;
	int64_t num = 10;
	FILE *fp;

	while ((opt = getopt(argc, argv, "n:0123456789")) != -1) {
		switch (opt) {
		/* Support legacy BSD style -num options. */
		case '0': case '1': case '2': case '3': case '4':
		case '5': case '6': case '7': case '8': case '9':
			bsdopt = optind;
			break;
		case 'n':
			if ((num = x_to_num(optarg, DEC)) < 0)
				die(1, "head: '%lld': argument is negative", num);
			bsdopt = 0;
			break;
		default:
			usage();
			break;
		}
	}
	if (bsdopt != 0)
		num = x_to_num(argv[bsdopt - 1] + 1, DEC);
	argc -= optind;
	argv += optind;

	if (argc == 0)
		return !!head("stdin", stdin, num);

	for (int i = 0; i < argc; ++i) {
		if (!(fp = !strcmp(argv[i], "-") ? stdin : fopen(argv[i], "r"))) {
			warn("head: fopen '%s':", argv[i]);
			ret = 1;
			continue;
		}
		if (argc > 1) {
			if (i > 1)
				fputc('\n', stdout);
			printf("==> %s <==\n", (fp == stdin) ? "stdin" : argv[i]);
		}
		if (head(argv[i], fp, num))
			ret = 1;
		if (fp != stdin)
			fclose(fp);
	}

	return ret;
}

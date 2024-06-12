/*
 * Copyright 2023-2024 Tom Schwindl <schwindl@posteo.de>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS “AS
 * IS” AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *
 * head - print the first few lines of the input files
 * ref: https://pubs.opengroup.org/onlinepubs/9699919799/utilities/head.html
 */
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
head(char const *const file, FILE *const fp, int64_t const num)
{
	ssize_t n;
	size_t len = 0;
	char *line = NULL;

	for (int64_t i = 0; i < num && (n = getline(&line, &len, fp)) != -1; ++i)
		fwrite(line, sizeof (char), n, stdout);
	free(line);

	if (ferror(fp)) {
		warn("getline '%s':", file);
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

	setup("head", argv);
	while ((opt = getopt(argc, argv, "n:0123456789")) != -1) {
		switch (opt) {
		/* Support legacy BSD style -num options. */
		case '0': case '1': case '2': case '3': case '4':
		case '5': case '6': case '7': case '8': case '9':
			bsdopt = optind;
			break;
		case 'n':
			if ((num = x_to_num(optarg, DEC)) < 0)
				die(1, "%s: '%lld': argument must be positive", _prog, num);
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
		return !!head("-", stdin, num);

	for (int i = 0; i < argc; ++i) {
		if (!(fp = !strcmp(argv[i], "-") ? stdin : fopen(argv[i], "r"))) {
			warn("fopen '%s':", argv[i]);
			ret = 1;
			continue;
		}
		if (argc > 1)
			printf("%s==> %s <==\n", (i > 1) ? "\n" : "", argv[i]);
		if (head(argv[i], fp, num))
			ret = 1;
		if (fp != stdin)
			fclose(fp);
	}

	return ret;
}

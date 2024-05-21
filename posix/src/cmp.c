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
 * cmp - compare two files byte by byte
 * ref: https://pubs.opengroup.org/onlinepubs/9699919799/utilities/cmp.html
 */
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "util.h"

static bool lflag = false;
static bool sflag = false;

static void
usage(void)
{
	die(2, "usage: cmp [-l | -s] file1 file2");
}

static int
compare_files(char *fn[], FILE *fp[])
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
	if (fn[2])
		warn("cmp: EOF on %s after byte %zu", fn[2], n-1);

	return (c[0] != c[1]);
}

int
main(int argc, char *argv[])
{
	int opt;
	char *fn[3] = {NULL};
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
		fp[i] = (!strcmp(fn[i], "-")) ? stdin : x_fopen(fn[i], "r");
	}

	/* Is the file compared to itself? */
	if (!strcmp(fn[0], fn[1]))
		return 0;

	return compare_files(fn, fp);
}

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
compare_files(char const *const fn[static 2], FILE *const fp[static 2])
{
	size_t bytes = 1;
	size_t ln    = 1;
	size_t i     = 0;
	size_t n[2];
	char b1[BUFSIZ] = {0};
	char b2[BUFSIZ] = {0};

	while (!feof(fp[0]) && !feof(fp[1])) {
		n[0] = fread(b1, sizeof (char), BUFSIZ, fp[0]);
		n[1] = fread(b2, sizeof (char), BUFSIZ, fp[1]);
		if (ferror(fp[0]) || ferror(fp[1])) {
			warn("fread '%s':", ferror(fp[0]) ? fn[0] : fn[1]);
			break;
		}

		for (i = 0; i < MIN(n[0], n[1]); ++i, ++bytes) {
			if (b1[i] == '\n')
				++ln;
			if (b1[i] != b2[i]) {
				if (sflag) {
					goto leave;
				} else if (lflag) {
					printf("%zu %o %o\n", bytes, b1[i], b2[i]);
				} else {
					printf("%s %s differ: byte %zu, line %zu\n", \
					        fn[0], fn[1], bytes, ln);
					goto leave;
				}
			}
		}

		if (n[0] != n[1]) {
			warn("EOF on %s after byte %zu", \
			    (n[0] < n[1]) ? fn[0] : fn[1], bytes - 1);
			break;
		}
	}

leave:
	return (b1[i] != b2[i]);
}

int
main(int argc, char *argv[])
{
	int opt;
	char const *fn[2];
	FILE *fp[2];

	x_setup("cmp", argv);
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

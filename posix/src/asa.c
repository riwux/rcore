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
 * ref: https://pubs.opengroup.org/onlinepubs/9699919799/utilities/asa.html
 * ref: https://ibiblio.org/pub/languages/fortran/ch2-14.html
 */
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "util.h"

static void
usage(void)
{
	die(1, "usage: asa [file...]");
}

static int
asa(char const *const file, FILE *const fp)
{
	bool first = true;
	ssize_t n;
	size_t len = 0;
	char *line = NULL;

	while ((n = getline(&line, &len, fp)) > 0) {
		if (line[--n] == '\n')
			line[n] = '\0';
		switch (line[0]) {
		case '0':
			if (!first)
				putchar('\n');
			putchar('\n');
			break;
		case '1':
			putchar('\f');
			break;
		case '+':
			if (!first)
				putchar('\r');
			break;
		default: /* <space> */
			if (!first)
				putchar('\n');
			break;
		}
		fwrite(line + 1, sizeof (char), n, stdout);
		first = false;
	}
	putchar('\n');

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
	int ret = 0;
	FILE *fp;

	x_setup("asa", argv);
	while ((opt = getopt(argc, argv, "")) != -1) {
		switch (opt) {
		default:
			usage();
			break;
		}
	}
	argc -= optind;
	argv += optind;

	if (argc == 0)
		*--argv = "-";
	for (; *argv; ++argv) {
		if (!(fp = !strcmp(*argv, "-") ? stdin : fopen(*argv, "r"))) {
			ret = 1;
			warn("fopen '%s':", *argv);
			continue;
		}
		if (asa(*argv, fp))
			ret = 1;
		if (fp != stdin)
			fclose(fp);
	}

	return ret;
}

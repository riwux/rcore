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
 * This file implements utility functions that simplify or wrap often needed
 * I/O operations.
 */
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "util.h"

static void
vwarn(char const *const fmt, va_list args)
{
	if (!fmt)
		return;

	vfprintf(stderr, fmt, args);
	if (fmt[0] && fmt[strlen(fmt) - 1] == ':') {
		fputc(' ', stderr);
		perror(NULL);
	} else {
		fputc('\n', stderr);
	}
}

void
warn(char const *const fmt, ...)
{
	fprintf(stderr, "%s: ", _prog);
	va_list args;
	va_start(args, fmt);
	vwarn(fmt, args);
	va_end(args);
}

void
die(int const status, char const *const fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	vwarn(fmt, args);
	va_end(args);
	exit(status);
}

ssize_t
non_block_fread(char *const buf, size_t const bsize, FILE *const fp)
{
	int fd;

	if ((fd = fileno(fp)) == -1)
		return -1;

	return read(fd, buf, bsize);
}

int
fcopy(FILE *const out_fp, FILE *const in_fp)
{
	ssize_t n;
	char buf[BUFSIZ];

	while ((n = non_block_fread(buf, BUFSIZ, in_fp)) > 0) {
		fwrite(buf, n, 1, out_fp);
		if (ferror(out_fp)) {
			n = -1;
			break;
		}
	}
	if (ferror(in_fp))
		n = -1;

	return (n < 0) ? -1 : 0;
}

FILE *
x_fopen(char const *const path, char const *const mode)
{
	FILE *fp;

	if (!(fp = fopen(path, mode)))
		die(1, "%s: fopen '%s':", _prog, path);

	return fp;
}

void
x_fclose(FILE *const fp)
{
	if (fp != NULL && fclose(fp) == EOF)
		die(1, "%s: fclose:", _prog);
}

/* See LICENSE file for copyright and license details. */
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "util.h"

static int
vwarn(char const *fmt, va_list args)
{
	if (!fmt)
		return -1;
	vfprintf(stderr, fmt, args);

	if (fmt[0] && fmt[strlen(fmt)-1] == ':') {
		fputc(' ', stderr);
		perror(NULL);
	} else {
		fputc('\n', stderr);
	}

	return 0;
}

void
warn(char const *fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	if (vwarn(fmt, args))
		return;
	va_end(args);
}

void
die(int status, char const *fmt, ...)
{

	va_list args;
	va_start(args, fmt);
	vwarn(fmt, args);
	va_end(args);

	exit(status);
}

int
fcopy(FILE *out_fp, FILE *in_fp, size_t bsize)
{
	ssize_t n = -1;
	char *buf;

	buf = x_malloc(bsize, sizeof (char));
	while (!feof(in_fp)) {
		n = fread(buf, sizeof (char), bsize, in_fp);
		fwrite(buf, sizeof (char), n, out_fp);
		if (ferror(out_fp) || ferror(in_fp)) {
			n = -1;
			break;
		}
	}
	free(buf);

	return (n < 0) ? 1 : 0;
}

FILE *
x_fopen(char const *path, char const *mode)
{
	FILE *fp;

	if (!(fp = fopen(path, mode)))
		die(1, "fopen: cannot open '%s':", path);

	return fp;
}

void
x_fclose(FILE *fp)
{
	if (fp != NULL && fclose(fp) == EOF)
		die(1, "fclose:");
}

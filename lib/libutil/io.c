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
	if (vwarn(fmt, args))
		return;
	va_end(args);

	exit(status);
}

int
fcopy(FILE *out_fp, FILE *in_fp, size_t bsize)
{
	ssize_t n;
	char *buf;

	buf = x_malloc(bsize, sizeof (char));
	while ((n = fread(buf, sizeof (char), bsize, in_fp)) > 0) {
		if (fwrite_all(out_fp, buf, n) == -1) {
			n = -1;
			break;
		}
	}
	free(buf);

	return (n < 0) ? 1 : 0;
}

ssize_t
fwrite_all(FILE *fp, char const *buf, size_t count)
{
	size_t n = 0;
	size_t i = n;

	while (count) {
		n = fwrite(buf + i, 1, count, fp);
		if (ferror(fp))
			return -1;
		i     += n;
		count -= n;
	}

	return count;
}

FILE *
x_fopen(char const *path, char const *mode)
{
	FILE *fp;

	if (!(fp = fopen(path, mode)))
		die(1, "fopen:");

	return fp;
}

void
x_fclose(FILE *fp)
{
	if (fp != NULL && fclose(fp) == EOF)
		die(1, "fclose:");
}

/* See LICENSE file for copyright and license details. */
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "util.h"

static void
vwarn(char const *fmt, va_list args)
{
	if (!fmt)
		return;

	vfprintf(stderr, fmt, args);
	if (fmt[0] && fmt[strlen(fmt)-1] == ':') {
		fputc(' ', stderr);
		perror(NULL);
	} else {
		fputc('\n', stderr);
	}
}

void
warn(char const *fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	vwarn(fmt, args);
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

ssize_t
non_block_fread(char *buf, size_t bsize, FILE *fp)
{
	int fd;

	if ((fd = fileno(fp)) == -1)
		return -1;
	return read(fd, buf, bsize);
}

int
fcopy(FILE *out_fp, FILE *in_fp)
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

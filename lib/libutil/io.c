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
fread_nb(char *buf, size_t bsize, FILE *fp)
{
	int fd;
	ssize_t n;

	if ((fd = fileno(fp)) == -1)
		return -1;

	n = read(fd, buf, bsize);
	return n;
}

int
fcopy(FILE *out_fp, FILE *in_fp)
{
	int in_fd;
	int out_fd;
	ssize_t n;
	char *buf  = x_malloc(BUFSIZ, sizeof (char));

	if ((in_fd = fileno(in_fp)) == -1 || (out_fd = fileno(out_fp)) == -1)
		return -1;

	while ((n = read(in_fd, buf, BUFSIZ)) > 0) {
		if (write(out_fd, buf, n) == -1) {
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

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
fcopy(FILE *out_fp, FILE *in_fp)
{
	int in_fd  = fileno(in_fp);
	int out_fd = fileno(out_fp);
	ssize_t n  = -1;
	char *buf  = x_malloc(BUFSIZ, sizeof (char));

	if (in_fd == -1 || out_fd == -1)
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

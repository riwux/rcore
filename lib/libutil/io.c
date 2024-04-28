/* See LICENSE file for copyright and license details. */
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "util.h"

void
eprintf(char const *fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	vfprintf(stderr, fmt, args);
	va_end(args);
}

void
die(int status, char const *fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	vfprintf(stderr, fmt, args);
	va_end(args);

	if (fmt[0] && fmt[strlen(fmt)-1] == ':') {
		fputc(' ', stderr);
		perror(NULL);
	} else {
		fputc('\n', stderr);
	}

	exit(status);
}

int
copy_file(int out_fd, int in_fd)
{
	ssize_t n;
	char *buf;

	buf = xmalloc(BUFCAP, sizeof char);
	while ((n = read(in_fd, buf, BUFCAP)) > 0)
		xwrite_all(out_fd, buf, n);
	free(buf);

	return (n < 0) ? 1 : 0;
}

ssize_t
write_all(int fd, char const *buf, size_t count)
{
	ssize_t n = 0;
	ssize_t i = n;

	while (count) {
		if ((n = write(fd, buf+i, count)) == -1)
			return -1;
		count -= n;
		i += n;
	}

	return count;
}

void
xwrite_all(int fd, char const *buf, size_t count)
{
	if (write_all(fd, buf, count) == -1)
		die(1, "write_all:");
}

void
xclose(int fd)
{
	if (fd != -1 && close(fd) < 0)
		die(1, "close:");
}

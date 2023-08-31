/* See LICENSE file for copyright and license details. */
#include <stdlib.h>
#include <unistd.h>

#include "util.h"

void *
xmalloc(size_t size)
{
	void *buf;

	if (!(buf = malloc(size)))
		die(FAILURE, "malloc:");

	return buf;
}

void *
xrealloc(void *buf, size_t size)
{
	if (!(buf = realloc(buf, size)))
		die(FAILURE, "realloc:");

	return buf;
}

void
xwrite_all(int fd, void *buf, size_t count)
{
	if (write_all(fd, buf, count) == -1)
		die(FAILURE, "write_all:");
}

void
xclose(int fd)
{
	if (fd != -1 && close(fd) < 0)
		die(FAILURE, "close:");
}

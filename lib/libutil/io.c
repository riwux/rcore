/* See LICENSE file for copyright and license details. */
#include <stdio.h>
#include <unistd.h>

#include "util.h"

/* FIXME: awfully inefficient */
ssize_t
get_line(int fd, Buf *buf)
{
	ssize_t n;
	ssize_t ret = 0;
	size_t i = 0;
	char c;

	while ((n = read(fd, &c, 1)) > 0) {
		if ((ret += n) >= buf->size) {
			buf->size *= 2;
			buf->data = xrealloc(buf->data, buf->size);
		}

		buf->data[i++] = c;
		if (c == '\n') {
			buf->data[i] = '\0';
			break;
		}
	}
	if (n == -1)
		ret = -1;
	else if (n == 0)
		ret = 0;

	return ret;
}

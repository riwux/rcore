/* See LICENSE file for copyright and license details. */
#include <stdio.h>
#include <unistd.h>

#include "util.h"

int
output_file(int fd)
{
	ssize_t n;
	Buf *buf = buf_create(BUFLEN);

	while ((n = read(fd, buf->data, buf->size)) > 0)
		xwrite_all(STDOUT_FILENO, buf->data, n);
	buf_free(buf);

	return (n < 0) ? 1 : 0;
}

ssize_t
write_all(int fd, void *buf, size_t count)
{
	ssize_t n = 0;
	ssize_t i = n;

	while (count) {
		if ((n = write(fd, (char *)buf+i, count)) == -1)
			return n;
		count -= n;
		i += n;
	}

	return count;
}

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
	if (n <= 0)
		ret = n;

	return ret;
}


/*
 * Function wrappers that include error handling.
 */

void
xwrite_all(int fd, void *buf, size_t count)
{
	if (write_all(fd, buf, count) == -1)
		die("write_all:");
}

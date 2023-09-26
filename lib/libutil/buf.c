/* See LICENSE file for copyright and license details. */
#include <stdlib.h>

#include "util.h"

struct Buf *
buf_create(size_t size)
{
	struct Buf *buf;

	buf = xmalloc(sizeof(*buf));
	buf->data = xmalloc(size);
	buf->size = size;

	return buf;
}

void
buf_free(struct Buf *buf)
{
	free(buf->data);
	free(buf);
}

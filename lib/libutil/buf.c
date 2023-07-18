/* See LICENSE file for copyright and license details. */
#include <stdlib.h>

#include "util.h"

Buf *
buf_create(size_t size)
{
	Buf *buf;

	buf = xmalloc(sizeof(*buf));
	buf->data = xmalloc(size);
	buf->size = size;

	return buf;
}

void
buf_free(Buf *buf)
{
	free(buf->data);
	free(buf);
	buf = NULL;
}

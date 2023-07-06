/* See LICENSE file for copyright and license details. */
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

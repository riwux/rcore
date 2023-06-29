/* See LICENSE file for copyright and license details. */
#include "util.h"

Buf *
buf_create(size_t size)
{
	Buf *buf;

	buf = emalloc(sizeof(*buf));
	buf->data = emalloc(size);
	buf->size = size;

	return buf;
}

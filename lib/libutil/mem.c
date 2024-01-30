/* See LICENSE file for copyright and license details. */
#include <stdlib.h>

#include "util.h"

void *
xmalloc(size_t size)
{
	void *buf;

	if (!(buf = malloc(size)))
		die(1, "malloc:");

	return buf;
}

void *
xrealloc(void *buf, size_t size)
{
	if (!(buf = realloc(buf, size)))
		die(1, "realloc:");

	return buf;
}

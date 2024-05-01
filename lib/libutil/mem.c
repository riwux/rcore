/* See LICENSE file for copyright and license details. */
#include <errno.h>
#include <stdlib.h>

#include "util.h"

void *
x_malloc(size_t nmemb, size_t size)
{
	void *p;

	if (overflow_mul(nmemb, size)) {
		errno = ENOMEM;
		die(1, "malloc:");
	}

	if (!(p = malloc(size * nmemb)))
		die(1, "malloc:");

	return p;
}

void *
x_realloc(void *p, size_t nmemb, size_t size)
{
	if (overflow_mul(nmemb, size)) {
		errno = ENOMEM;
		die(1, "realloc:");
	}

	if (!(p = realloc(p, nmemb * size)))
		die(1, "realloc:");

	return p;
}

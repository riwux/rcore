/* See LICENSE file for copyright and license details. */
#include <errno.h>
#include <stdint.h>
#include <stdlib.h>

#include "util.h"

int_least64_t
to_num(char *str, int base)
{
	int_least64_t ret;

	errno = 0;
	ret = strtoll(str, NULL, base);
	if (errno)
		die(1, "strtoll:");

	return ret;
}

uint_least64_t
to_unum(char *str, int base)
{
	uint_least64_t ret;

	errno = 0;
	ret = strtoull(str, NULL, base);
	if (errno)
		die(1, "strtoull:");

	return ret;
}

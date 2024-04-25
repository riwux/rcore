/* See LICENSE file for copyright and license details. */
#include <errno.h>
#include <stdint.h>
#include <stdlib.h>

#include "util.h"

int64_t
to_num(char const *str, int base)
{
	int64_t ret;

	errno = 0;
	ret = strtoll(str, NULL, base);
	if (errno)
		die(1, "strtoll:");

	return ret;
}

uint64_t
to_unum(char const *str, int base)
{
	uint64_t ret;

	errno = 0;
	ret = strtoull(str, NULL, base);
	if (errno)
		die(1, "strtoull:");

	return ret;
}

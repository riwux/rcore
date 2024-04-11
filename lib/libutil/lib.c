/* See LICENSE file for copyright and license details. */
#include <string.h>

#include "util.h"

char
unescape(char *arg)
{
	char *p;
	const char *escape = "a\ab\bf\fn\nr\rt\tv\v\\\\";

	p = strchr(escape, *arg);
	return (!p ? *arg : p[1]);
}

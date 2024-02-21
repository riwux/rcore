/* See LICENSE file for copyright and license details. */
#include <string.h>

#include "util.h"

char
unescape(char *arg)
{
	char *p;

	const char *escape = "a\a"
	                     "b\b"
	                     "f\f"
	                     "n\n"
	                     "r\r"
	                     "t\t"
	                     "v\v"
	                     "\\\\";

	p = strchr(escape, *arg);
	return (!p ? *arg : p[1]);
}

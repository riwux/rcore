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

size_t
unescape_num(char *str)
{
	char c;
	int i;
	size_t len;
	char *p;
	char num[4];

	p   = str;
	len = 0;

	while ((c = *str)) {
		++len;
		if (*str == '\\' && str[1]) {
			num[0] = num[1] = num[2] = num[3] = 0;
			switch (*++str) {
			case 'x':
				if (!is_hexdigit(*++str)) {
					--str;
					break;
				}
				for (i=0; i<2 && is_hexdigit(*str); ++i, ++str)
					num[i] = *str;
				c = to_num(num, HEX);
				break;
			case '0':
				for (i=0; i<4 && is_octdigit(*str); ++i, ++str)
					num[i] = *str;
				c = to_num(num, OCT);
				break;
			case 'a': case 'b':
			case 'f': case 'n':
			case 'r': case 't':
			case 'v': case '\\':
				c = unescape(str++);
				break;
			default:
				c = '\\';
				break;
			}
		} else {
			++str;
		}
		*p++ = c;
	}
	*p = '\0';

	/* Since octal escapes might embed NUL-bytes, the length is mandatory. */
	return len;
}

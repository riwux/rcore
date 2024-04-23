/* See LICENSE file for copyright and license details. */
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "util.h"

/*
 * Local support functions.
 */

static mode_t
apply_mode(char op, mode_t old, mode_t who, mode_t perm)
{
	switch (op) {
	case '+':
		old |= (who & perm);
		break;
	case '-':
		old &= ~(who & perm);
		break;
	case '=':
		old = (who & perm) | (~who & old);
		break;
	default:
		break;
	}
	return old;
}


/*
 * Public interface.
 */

char
unescape(char *arg)
{
	char *p;
	const char *escape = "a\ab\bf\fn\nr\rt\tv\v\\\\";

	p = strchr(escape, *arg);
	return (!p ? *arg : p[1]);
}

mode_t
parse_mode(char *mode)
{
	char op = 0;
	mode_t m, who;
	mode_t perm = 0;
	mode_t ret  = 0666;
	long oct;
	char *end;

	umask(0);

	/* 'mode' might already be an octal integer. */
	oct = strtol(mode, &end, OCT);
	if (*end == '\0') {
		if (oct > 07777 || oct < 0)
			die(1, "parse_mode: invalid mode '%s'", mode);
		return oct;
	}

	do {
		/* Each clause of a symbolic mode can have an optional wholist. */
		for (who = 0; *mode && !strchr("+-=", *mode); ++mode) {
			switch (*mode) {
			case 'a':
				who |= S_IRWXU | S_IRWXG | S_IRWXO | S_ISUID | S_ISGID | S_ISVTX;
				break;
			case 'u':
				who |= S_IRWXU | S_ISUID;
				break;
			case 'g':
				who |= S_IRWXG | S_ISGID;
				break;
			case 'o':
				who |= S_IRWXO;
				break;
			default:
				die(1, "parse_mode: invalid who symbol '%c'", *mode);
				break;
			}
		}
		if (!*mode)
			die(1, "parse_mode: missing operator");

		/* No wholist is equivalent to a wholist of 'a'.*/
		if (!who)
			who = S_IRWXU | S_IRWXG | S_IRWXO | S_ISUID | S_ISGID | S_ISVTX;

		/*
		 * Either of the following sequences/actionlists is allowed
		 * and can be repeated (as well as interchanged) indefinitely.
		 * op            | +-=
		 * op permlist   | +-=rwxXst
		 * op permcopy   | +-=ugo
		 */
		for (; *mode && *mode != ','; ++mode) {
			switch (*mode) {
			case '+':
			case '-':
			case '=':
				/* No permission symbols are given. */
				if (op) {
					ret  = apply_mode(op, ret, who, perm);
					perm = 0;
				}
				op = *mode;
				break;
			case 'r':
				perm |= S_IRUSR | S_IRGRP | S_IROTH;
				break;
			case 'w':
				perm |= S_IWUSR | S_IWGRP | S_IWOTH;
				break;
			case 'x':
				perm |= S_IXUSR | S_IXGRP | S_IXOTH;
				break;
			case 'X':
				/*
				 * Only set the execute bit if the file to be modified is a
				 * directory or if the already present permissions have at
				 * last one execute bit set (XUSR|XGRP|XOTH). This will
				 * require at least one more argument to parse_mode().
				 */
				break;
			case 's':
				if (who & S_ISGID)
					perm |= S_ISGID;
				if (who & S_ISUID)
					perm |= S_ISUID;
				break;
			case 't':
				perm |= S_ISVTX;
				break;
			case 'u':
				m = (ret & S_IRWXU);
				if (who & S_IRWXU)
					perm |= m;
				if (who & S_IRWXG)
					perm |= m >> 3;
				if (who & S_IRWXO)
					perm |= m >> 6;
				break;
			case 'g':
				m = (ret & S_IRWXG);
				if (who & S_IRWXU)
					perm |= m << 3;
				if (who & S_IRWXG)
					perm |= m;
				if (who & S_IRWXO)
					perm |= m >> 3;
				break;
			case 'o':
				m = (ret & S_IRWXO);
				if (who & S_IRWXU)
					perm |= m << 6;
				if (who & S_IRWXG)
					perm |= m << 3;
				if (who & S_IRWXO)
					perm |= m;
				break;
			default:
				die(1, "parse_mode: invalid permission symbol '%c'", *mode);
				break;
			}
		}
		ret  = apply_mode(op, ret, who, perm);
		perm = 0;
		op   = 0;
	} while (*mode == ',' && *++mode);

	/* Make sure only necessary bits are returned. */
	return (ret & ~S_IFMT);
}

mode_t
get_umask(void)
{
	mode_t ret = umask(0);
	umask(ret);
	return ret;
}

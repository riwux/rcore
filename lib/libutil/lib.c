/*
 * Copyright 2023-2024 Tom Schwindl <schwindl@posteo.de>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS “AS
 * IS” AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *
 * This file implements general utility functions that are commonly used
 * thoughout the entire codebase to simplify complex, but often occuring
 * situations which don't really fit in a more concrete category.
 */
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "util.h"

static mode_t
apply_mode(char const op, mode_t old, mode_t const who, mode_t const perm)
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

mode_t
x_parsemode(char const *mode, mode_t const init)
{
	char op     = 0;
	mode_t perm = 0;
	mode_t ret  = init;
	mode_t m, who;
	long oct_mode;
	char *inval;

	/* 'mode' might already be a valid octal integer. */
	oct_mode = strtol(mode, &inval, OCT);
	if (!*inval) {
		if (oct_mode > 07777 || oct_mode < 0)
			die(1, "%s: parsemode '%s': invalid mode", prog_, mode);
		return oct_mode;
	}

	/*
	 * Take the mode 'o+r' with a default value of zero as an example.
	 *
	 * By setting all bits of the specified who's ([o]ther in this case)
	 * to one, they are selected for modification.
	 *
	 *        0000 0000 0000 0000  |  S_IRWXO (0007)
	 *        0000 0000 0000 0111
	 *
	 * Now the actual permissions ([r]ead in this case) are applied to
	 * the previously calculated bitmask by blindly ANDing them to all (ugo)
	 * who's. Since only the previously selected who's have a value of one,
	 * the bits of all the others become zero immediately. Then all the bits
	 * of the selected who's that do not represent a specified permission
	 * turn zero too.
	 *
	 *        0000 0000 0000 0111     (bitmask [0007])
	 *        0000 0001 0010 0100     (S_IRUSR | S_IRGRP | S_IROTH [0444])
	 *     &  ___________________
	 *        0000 0000 0000 0100
	 *
	 * This yields a pattern that has all bits at the right place.
	 * Depending on the operator, additional calculations might be necessary
	 * to receive a correct mode (see apply_mode() for details).
	 */
	do {
		/* A list of who's can be provided for each clause. */
		for (who = 0; *mode && !strchr("+-=", *mode); ++mode) {
			switch (*mode) {
			case 'a':
				who |= S_IRWXU | S_IRWXG | S_IRWXO | S_ISUID | S_ISGID \
				    | S_ISVTX;
				break;
			case 'u':
				who |= S_IRWXU | S_ISUID;
				break;
			case 'g':
				who |= S_IRWXG | S_ISGID;
				break;
			case 'o':
				who |= S_IRWXO | S_ISVTX;
				break;
			default:
				die(1, "%s: parsemode: invalid who symbol '%c'", prog_, *mode);
				break;
			}
		}
		if (!*mode)
			die(1, "%s: parsemode: missing operator", prog_);
		if (!who)
			who = S_IRWXU | S_IRWXG | S_IRWXO | S_ISUID | S_ISGID | S_ISVTX;

		/*
		 * The following actionlists are valid and can be
		 * repeated, as well as interchanged, indefinitely.
		 *
		 * op            ->   +-=
		 * op permlist   ->   +-=rwxXst
		 * op permcopy   ->   +-=ugo
		 */
		for (; *mode && *mode != ','; ++mode) {
			switch (*mode) {
			case '+':
			case '-':
			case '=':
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
				if (ret & (S_IXUSR | S_IXGRP | S_IXOTH))
					perm |= S_IXUSR | S_IXGRP | S_IXOTH;
				break;
			case 's':
				perm |= S_ISUID | S_ISGID;
				break;
			case 't':
				perm |= S_ISVTX;
				break;
			case 'u':
				m = (ret & S_IRWXU);
				perm |= (m >> 3) | (m >> 6) | m;
				break;
			case 'g':
				m = (ret & S_IRWXG);
				perm |= (m << 3) | (m >> 3) | m;
				break;
			case 'o':
				m = (ret & S_IRWXO);
				perm |= (m << 6) | (m << 3) | m;
				break;
			default:
				die(1, "%s: parsemode: invalid permission symbol '%c'", prog_, \
				    *mode);
				break;
			}
		}
		ret  = apply_mode(op, ret, who, perm);
		perm = 0;
		op   = 0;
	} while (*mode == ',' && *++mode);

	return ret;
}

mode_t
get_umask(void)
{
	mode_t const ret = umask(0);
	umask(ret);
	return ret;
}

char
unescape(char const *const arg)
{
	char const *p;
	char const *const escape = "a\ab\bf\fn\nr\rt\tv\v\\\\";

	p = strchr(escape, *arg);
	return (!p ? *arg : p[1]);
}

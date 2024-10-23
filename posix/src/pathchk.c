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
 * ref: https://pubs.opengroup.org/onlinepubs/9699919799/utilities/pathchk.html
 */
#include <errno.h>
#include <limits.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>

#include "util.h"

#define CHARSET ("ABCDEFGHIJKLMNOPQRSTUVWXYZ" \
                "abcdefghijklmnopqrstuvwxyz0123456789._-")

static bool pflag = false;
static bool Pflag = false;

static void
usage(void)
{
	die(1, "usage: pathchk [-p] [-P] path...");
}

static int
check_path(char *const path)
{
	int ret     = 0;
	size_t clen = 0;
	size_t const plen    = strlen(path);
	size_t const maxpath = (pflag ? _POSIX_PATH_MAX : PATH_MAX);
	size_t const maxname = (pflag ? _POSIX_NAME_MAX : NAME_MAX);
	char const *start;
	struct stat st;

	if (Pflag && plen == 0) {
		warn("pathname is empty");
		return -1;
	} else if (plen > maxpath) {
		warn("limit '%zu' exceeded by length '%zu' of pathname '%s'", \
		    maxpath, plen, path);
		return -1;
	}

	/* Make sure every path ends in a trailing <slash>. */
	path[plen] = (path[plen - 1] != '/') ? '/' : path[plen];
	start      = path;

	for (char *p = path; ret != -1 && p <= &path[plen]; ++p) {
		if (*p != '/')
			continue;
		*p    = '\0';
		clen  = p - start;
		errno = 0;
		if (clen > maxname) {
			warn("limit '%zu' exceeded by length '%zu' " \
			    "of pathname component '%s'", maxname, clen, start);
			ret = -1;
		/*
		 * lstat(2) doesn't dereference symbolic links which is important
		 * for checking the name of the link _itself_. It also fails if one
		 * of the files in pathname doesn't have execute permissions.
		 * Only existing files are considered.
		 */
		} else if (lstat(start, &st) && errno != ENOENT) {
			warn("pathname component doesn't have search permissions: '%s'", \
			    start);
			ret = -1;
		} else if (Pflag && (start[0] == '-')) {
			warn("pathname component starts with <hyphen-minus>: '%s'", start);
			ret = -1;
		} else if (pflag) {
			for (char const *np = start; np < p; ++np) {
				if (!strchr(CHARSET, *np)) {
					warn("pathname component contains " \
					    "non-portable character '%c': '%s'", *np, start);
					ret = -1;
					break;
				}
			}
		}
		*p = '/';
		start = p + 1;
	}

	return ret;
}

int
main(int argc, char *argv[])
{
	int ret = 0;
	int opt;

	x_setup("pathchk", argv);
	while ((opt = getopt(argc, argv, "pP")) != -1) {
		switch (opt) {
		case 'p':
			/*
			 * Are the Pathname and its components..
			 * ..longer than _POSIX_PATH_MAX?
			 * ..longer than _POSIX_NAME_MAX?
			 * ..characters part of the "portable filename character set"?
			 */
			pflag = true;
			break;
		case 'P':
			/*
			 * Is the first character of a pathname component
			 * a <hyphen-minus>?
			 * Is the pathname empty?
			 */
			Pflag = true;
			break;
		default:
			usage();
			break;
		}
	}
	argc -= optind;
	argv += optind;

	if (argc == 0)
		usage();

	for (; *argv; ++argv) {
		if (check_path(*argv))
			ret = 1;
	}

	return ret;
}

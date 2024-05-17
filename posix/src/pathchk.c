/* See LICENSE file for copyright and license details. */
#include <errno.h>
#include <limits.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>

#include "util.h"

static bool pflag = false;
static bool Pflag = false;

static void
usage(void)
{
	die(1, "usage: pathchk [-p] [-P] path...");
}

static int
check_path(char const *pathname)
{
	int ret     = 0;
	size_t clen = 0;
	size_t plen = strlen(pathname);
	size_t const maxpath = (pflag ? _POSIX_PATH_MAX : PATH_MAX);
	size_t const maxname = (pflag ? _POSIX_NAME_MAX : NAME_MAX);
	char const *start;
	char const *const charset = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
	                            "abcdefghijklmnopqrstuvwxyz0123456789._-";
	char path[PATH_MAX + 2] = {0};
	struct stat st;

	if (Pflag && plen == 0) {
		warn("pathchk: pathname is empty");
		return -1;
	}
	if (plen > maxpath) {
		warn("pathchk: limit '%zu' exceeded by length " \
		    "'%zu' of pathname '%s'", maxpath, plen, pathname);
		return -1;
	}

	/* Make sure every path ends in a trailing <slash>. */
	memcpy(path, pathname, plen);
	path[plen] = (path[plen - 1] != '/') ? '/' : path[plen];
	start      = path;

	for (char *p = path; ret != -1 && p < &path[plen + 1]; ++p) {
		if (*p != '/')
			continue;
		*p   = '\0';
		clen = p - start;
		if (clen > maxname) {
			warn("pathchk: limit '%zu' exceeded by length '%zu' " \
			    "of pathname component '%s'", maxname, clen, start);
			return -1;
		}
		/*
		 * lstat(2) fails if one of the files in pathname doesn't
		 * have execute permissions. Only existing files are considered.
		 * There's no reason to dereference a potential symbolic link and
		 * check it since the link _itself_ is part of the path.
		 */
		errno = 0;
		if (lstat(start, &st) && errno != ENOENT) {
			warn("pathchk: pathname component doesn't have search " \
			    "permissions: '%s'", start);
			ret = -1;
		}
		if (pflag) {
			for (char const *np = start; np < p; ++np) {
				if (!strchr(charset, *np)) {
					warn("pathchk: pathname component contains " \
					    "non-portable character '%c': '%s'", *np, start);
					ret = -1;
					break;
				}
			}
		}
		if (Pflag && start[0] == '-') {
			warn("pathchk: pathname component starts with <hyphen-minus>: " \
			    "'%s'", start);
			ret = -1;
		}
		*p = '/';
		start = p + 1;
	}

	return ret;
}

int
main(int argc, char *argv[])
{
	int ret    = 0;
	int opt;

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
			 * Is the first character of a pathname component a <hyphen-minus>?
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

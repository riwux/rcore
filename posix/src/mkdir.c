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
 * mkdir - create directoies
 * ref: https://pubs.opengroup.org/onlinepubs/9699919799/utilities/mkdir.html
 */
#include <errno.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>

#include "util.h"

static void
usage(void)
{
	die(1, "usage: mkdir [-p] [-m mode] dir...");
}

static int
mkpath(char *path, mode_t mode)
{
	int ret = 0;
	char *p = (path[0] == '/') ? path + 1 : path; /* Skip root directory. */
	char *last;
	struct stat st;

	/* Make sure all trailing <slash>'es are stripped. */
	last = &path[strlen(path)];
	while (last > p && *--last == '/')
		*last = '\0';

	for (; ret != -1 && *p; ++p) {
		if (*p != '/')
			continue;
		*p = '\0';
		errno = 0;
		if (mkdir(path, 0) && errno != EEXIST) {
			warn("mkdir: mkdir '%s':", path);
			ret = -1;
		/* Already existing pathname components keep their mode. */
		} else if (errno != EEXIST &&
		    chmod(path, (S_IWUSR | S_IXUSR | ~get_umask()) & 0777)) {
			warn("mkdir: chmod '%s':", path);
			ret = -1;
		} else if (!stat(path, &st) && !S_ISDIR(st.st_mode)) {
			warn("mkdir: mkdir '%s': File exists but is not a directory", path);
			ret = -1;
		}
		*p = '/';
	}
	/*
	 * POSIX requires that in case a custom mode is provided, the new
	 * directory is not allowed to have a less restrictive mode than that
	 * at any point in time. Thus, the above call to mkdir(2) with a mode
	 * of zero and subsequent adjustments with chmod(2) wouldn't be compliant.
	 */
	 if (ret != -1 && mkdir(path, mode) && errno != EEXIST) {
		warn("mkdir: mkdir '%s':", path);
		ret = -1;
	}

	return ret;
}

int
main(int argc, char *argv[])
{
	bool pflag  = false;
	int opt;
	int ret     = 0;
	mode_t mode = ((S_IRWXU | S_IRWXG | S_IRWXO) & ~get_umask()) & 0777;

	while ((opt = getopt(argc, argv, "pm:")) != -1) {
		switch (opt) {
		case 'p':
			pflag = true;
			break;
		case 'm':
			mode = x_parsemode(optarg, (S_IRWXU | S_IRWXG | S_IRWXO));
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
		if (pflag) {
			if (mkpath(*argv, mode)) {
				ret = 1;
				continue;
			}
		} else if (mkdir(*argv, mode)) {
			warn("mkdir: mkdir '%s':", *argv);
			ret = 1;
			continue;
		}
		/*
		 * POSIX mkdir(2) only guarantees to handle the file permission bits and
		 * might ignore bits with a higher value (e.g. SUID, SGID, sticky...).
		 * Calling chmod(2) guarantees that all bits are applied correctly.
		 */
		if (errno != EEXIST && chmod(*argv, mode))
			warn("mkdir: chmod '%s':", *argv);
	}

	return ret;
}

/* See LICENSE file for copyright and license details. */
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "util.h"

static void
usage(void)
{
	die(1, "usage: tee [-ai] [file...]");
}

int
main(int argc, char *argv[])
{
	int ret, opt, flags;
	ssize_t n;
	int *fds;
	char *buf;

	ret   = 0;
	flags = O_WRONLY | O_CREAT | O_TRUNC;
	buf   = xmalloc(BUFCAP, sizeof (char));

	while ((opt = getopt(argc, argv, "ai")) != -1) {
		switch (opt) {
		case 'a':
			flags = (flags & ~O_TRUNC) | O_APPEND;
			break;
		case 'i':
			if (signal(SIGINT, SIG_IGN) == SIG_ERR)
				die(1, "signal:");
			break;
		default:
			usage();
			break;
		}
	}
	argc -= optind;
	argv += optind;
	fds = xmalloc(argc+1, sizeof *fds);

	for (int i = 0; i < argc; ++i) {
		fds[i] = open(argv[i], flags, 0666);
		if (fds[i] < 0) {
			perror(argv[i]);
			ret = 1;
		}
	}
	fds[argc]  = STDOUT_FILENO;
	argv[argc] = "stdout";

	while ((n = read(STDIN_FILENO, buf, BUFCAP)) > 0) {
		for (int i = 0; i <= argc; ++i) {
			if (fds[i] != -1 && write_all(fds[i], buf, n) == -1) {
				ret = 1;
				fds[i] = -1;
				perror(argv[i]);
			}
		}
	}
	free(buf);
	free(fds);

	return ret;
}

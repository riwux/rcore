/* See LICENSE file for copyright and license details. */
#include <fcntl.h>
#include <signal.h>
#include <unistd.h>
#include <stdio.h>

#include "util.h"

static void
usage(void)
{
	die(1, "usage: tee [-ai] [file...]");
}

int
main(int argc, char *argv[])
{
	int ret, opt;
	mode_t flags = O_WRONLY | O_CREAT | O_TRUNC;
	int *fds;
	ssize_t n;
	struct Buf *buf = buf_create(BUFLEN);

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
	fds = xmalloc(sizeof(*fds) * (argc+1));

	for (int i = 0; i < argc; ++i) {
		if ((fds[i] = open(argv[i], flags, 0666)) < 0) {
			perror(argv[i]);
			ret = 1;
		}
	}
	fds[argc] = STDOUT_FILENO;

	while ((n = read(STDIN_FILENO, buf->data, buf->size)) > 0) {
		for (int i = 0; i <= argc; ++i) {
			if (fds[i] != -1 && write_all(fds[i], buf->data, n) == -1) {
				ret = 1;
				fds[i] = -1;
			}
		}
	}
	buf_free(buf);

	return ret;
}

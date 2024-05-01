/* See LICENSE file for copyright and license details. */
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "util.h"

#define SKIP NULL

static void
usage(void)
{
	die(1, "usage: tee [-ai] [file...]");
}

int
main(int argc, char *argv[])
{
	char flags = 'w';
	int ret    = 0;
	int opt;
	size_t n;
	char *buf  = x_malloc(BUFSIZ, sizeof (char));
	FILE **fps;

	while ((opt = getopt(argc, argv, "ai")) != -1) {
		switch (opt) {
		case 'a':
			flags = 'a';
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
	fps = x_malloc(argc+1, sizeof *fps);

	for (int i = 0; i < argc; ++i) {
		if (!(fps[i] = fopen(argv[i], &flags))) {
			ret = 1;
			perror(argv[i]);
		}
	}
	fps[argc]  = stdout;
	argv[argc] = "stdout";

	while (!ferror(stdin) && !feof(stdin)) {
		n = fread(buf, sizeof (char), 1, stdin);
		for (int i = 0; i <= argc; ++i) {
			if (fps[i] != SKIP && fwrite_all(fps[i], buf, n) == -1) {
				fps[i] = SKIP;
				perror(argv[i]);
				ret = 1;
			}
		}
	}
	free(buf);
	free(fps);

	return ret;
}

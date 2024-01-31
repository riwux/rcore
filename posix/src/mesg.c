/* See LICENSE file for copyright and license details. */
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>

#include "util.h"

static void
usage(void)
{
	die(2, "usage: mesg [y|n]");
}

int
main(int argc, char *argv[])
{
	int opt, ret;
	mode_t mode;
	char *tty;
	struct stat st;

	while ((opt = getopt(argc, argv, "")) != -1) {
		switch (opt) {
		default:
			usage();
			break;
		}
	}
	argc -= optind;
	argv += optind;

	tty = ttyname(STDIN_FILENO);
	if (!tty)
		die(2, "ttyname:");
	if (stat(tty, &st))
		die(2, "stat:");

	switch (argc) {
	case 0:
		if (st.st_mode & S_IWGRP)
			puts("is y");
		else
			puts("is n");
		break;
	case 1:
		if (**argv == 'y' || **argv == 'Y')
			mode = st.st_mode | S_IWGRP;
		else if (**argv == 'n' || **argv == 'N')
			mode = st.st_mode & ~S_IWGRP;
		else
			die(2, "mesg: invalid argument '%c'", **argv);

		if (chmod(tty, mode))
			die(2, "chmod:");
		break;
	default:
		usage();
		break;
	}

	return !(st.st_mode & S_IWGRP);
}

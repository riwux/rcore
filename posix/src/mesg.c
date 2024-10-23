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
 * ref: https://pubs.opengroup.org/onlinepubs/9699919799/utilities/mesg.html
 */
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>

#include "util.h"

static void
usage(void)
{
	die(2, "usage: mesg [y | n]");
}

int
main(int argc, char *argv[])
{
	int opt;
	mode_t mode = 0;
	char const *tty;
	struct stat st;

	x_setup("mesg", argv);
	while ((opt = getopt(argc, argv, "")) != -1) {
		switch (opt) {
		default:
			usage();
			break;
		}
	}
	argc -= optind;
	argv += optind;

	tty = ttyname(STDERR_FILENO);
	if (!tty)
		die(2, "%s: ttyname 'STDERR_FILENO':", prog_);
	if (stat(tty, &st))
		die(2, "%s: stat '%s':", prog_, tty);

	switch (argc) {
	case 0:
		puts((st.st_mode & S_IWGRP) ? "is y" : "is n");
		break;
	case 1:
		if (**argv == 'y')
			mode = st.st_mode | S_IWGRP;
		else if (**argv == 'n')
			mode = st.st_mode & ~S_IWGRP;
		else
			die(2, "%s: invalid argument '%c'", prog_, **argv);
		if (chmod(tty, mode))
			die(2, "%s: chmod '%s':", prog_, tty);
		break;
	default:
		usage();
		break;
	}

	return !(st.st_mode & S_IWGRP);
}

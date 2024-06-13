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
 * ref: https://pubs.opengroup.org/onlinepubs/9699919799/utilities/tee.html
 */
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
	char const *flags = "w";
	int ret = 0;
	int opt;
	size_t n;
	char *buf;
	FILE **fps;

	setup("tee", argv);
	while ((opt = getopt(argc, argv, "ai")) != -1) {
		switch (opt) {
		case 'a':
			flags = "a";
			break;
		case 'i':
			if (signal(SIGINT, SIG_IGN) == SIG_ERR)
				die(1, "%s: signal:", _prog);
			break;
		default:
			usage();
			break;
		}
	}
	argc -= optind;
	argv += optind;
	buf = x_malloc(BUFSIZ, sizeof (char));
	fps = x_malloc(argc + 1, sizeof *fps);

	for (int i = 0; i < argc; ++i) {
		if (!(fps[i] = fopen(argv[i], flags))) {
			ret = 1;
			warn("fopen '%s':", argv[i]);
		}
	}
	fps[argc]  = stdout;
	argv[argc] = "stdout";

	while ((n = non_block_fread(buf, BUFSIZ, stdin)) > 0) {
		for (int i = 0; i <= argc; ++i) {
			if (fps[i]) {
				fwrite(buf, n, 1, fps[i]);
				if (ferror(fps[i])) {
					ret = 1;
					fps[i] = NULL;
					warn("fwrite '%s':", argv[i]);
				}
			}
		}
	}
	free(buf);
	free(fps);

	return ret;
}

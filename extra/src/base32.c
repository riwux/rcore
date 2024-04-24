/* See LICENSE file for copyright and license details. */
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "util.h"

static void
usage(void)
{
	die(1, "usage: base32 [-d] [file]");
}

int
main(int argc, char *argv[])
{
	bool dflag;
	int opt, n, i, s;
	uint64_t val;
	char buf[5];
	FILE *fp;

	val   = 0;
	dflag = false;
	const char *table = "ABCDEFGHIJKLMNOPQRSTUVWXYZ234567";

	while ((opt = getopt(argc, argv, "d")) != -1) {
		switch(opt) {
		case 'd':
			dflag = 1;
			break;
		default:
			usage();
			break;
		}
	}
	argc -= optind;
	argv += optind;

	if (argc > 1)
		usage();

	if (argc == 0 || !strcmp(*argv, "-")) {
		fp = stdin;
	} else {
		fp = fopen(*argv, "r");
		if (!fp)
			die(1, "fopen: '%s':", *argv);
	}

	if (dflag) {
		/* TODO: implement decoding */
		return 1;
	}

	while ((n = fread(&buf, sizeof(char), 5, fp)) > 0) {
		for (i = 0, s = 32; i < n; s -= 8, ++i)
			val |= (uint64_t)buf[i] << s;
		n = (n<4 ? n+2 : n+3);
		for (i = 0, s = 35; i < n; s -= 5, ++i)
			putchar(table[((val >> s) & 31)]); /* 2^5-1=31 extract 5 bits */
		val = 0;
	}
	for (i=8-i%8; i!=8 && i>0; --i)
		putchar('=');
	putchar('\n');
}

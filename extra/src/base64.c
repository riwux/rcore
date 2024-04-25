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
	die(1, "usage: base64 [-d] [file]");
}

int
main(int argc, char *argv[])
{
	bool dflag = false;
	int opt, n, i, s;
	uint32_t val = 0;
	char buf[3];
	FILE *fp;
	char const *const table = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
	                          "abcdefghijklmnopqrstuvwxyz"
	                          "0123456789+/";

	while ((opt = getopt(argc, argv, "d")) != -1) {
		switch (opt) {
		case 'd':
			dflag = true;
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

	while ((n = fread(&buf, sizeof(char), 3, fp)) > 0) {
		for (i = 0, s = 16; i < n; s -= 8, ++i)
			val |= (uint32_t)buf[i] << s;
		for (i = 0, s = 18; i <= n; s -= 6, ++i)
			putchar(table[((val >> s) & 63)]); /* 2^6-1=63 extract 6 bits. */
		val = 0;
	}
	for (i=4-i%4; i!=4 && i>0; --i)
		putchar('=');
	putchar('\n');
}

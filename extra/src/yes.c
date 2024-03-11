/* See LICENSE file for copyright and license details. */
#include <stdio.h>

int
main(int argc, char *argv[])
{
	if (argc < 2) {
		for (;;)
			puts("y");
	} else {
		for (;;) {
			for (int i = 1; i < argc; ++i)
				printf("%s%c", argv[i], (i == argc-1) ? '\n' : ' ');
		}
	}
}

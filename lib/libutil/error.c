/* See LICENSE file for copyright and license details. */
#include <stdio.h>
#include <string.h>

void
die(char *msg)
{
	if (msg[strlen(msg)-1] == ':') {
		fprintf(stderr, "%s ", msg);
		perror(NULL);
	} else {
		fputs(msg, stderr);
	}
}

void
tell(char *msg)
{
	fputs(msg, stderr);
}

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
 */
#define _BSD_SOURCE /* syscall(2) */

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <linux/reboot.h>

#include "util.h"

#define CAD_PATH "/proc/sys/kernel/ctrl-alt-del"

static void
usage(void)
{
	die(1, "usage: ctrlaltdel [hard | soft]");
}

int
main(int argc, char *argv[])
{
	char mode;
	int opt;
	uint64_t cmd = 0;
	FILE *fp;

	x_setup("ctrlaltdel", argv);
	while ((opt = getopt(argc, argv, "")) != -1) {
		switch (opt) {
		default:
			usage();
			break;
		}
	}
	argc -= optind;
	argv += optind;

	switch (argc) {
	case 0:
		fp = x_fopen(CAD_PATH, "r");
		fread(&mode, sizeof (char), 1, fp);
		puts((mode == '0') ? "soft" : "hard");
		x_fclose(fp);
		break;
	case 1:
		if (!strcmp(*argv, "soft"))
			cmd = LINUX_REBOOT_CMD_CAD_OFF;
		else if (!strcmp(*argv, "hard"))
			cmd = LINUX_REBOOT_CMD_CAD_ON;
		else
			die(1, "%s: invalid argument '%s'", prog_, *argv);
		if (syscall(SYS_reboot, LINUX_REBOOT_MAGIC1, LINUX_REBOOT_MAGIC2, \
		    cmd, NULL))
			die(1, "%s: reboot:", prog_);
		break;
	default:
		usage();
		break;
	}
}

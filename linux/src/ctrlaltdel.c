/* See LICENSE file for copyright and license details. */
#define _BSD_SOURCE /* syscall(2) */

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/reboot.h>
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
			die(1, "ctrlaltdel: invalid argument '%s'", *argv);
		if (syscall(SYS_reboot, LINUX_REBOOT_MAGIC1, LINUX_REBOOT_MAGIC2, \
		    cmd, NULL) != 0)
			die(1, "ctrlaltdel: reboot:");
		break;
	default:
		usage();
		break;
	}
}

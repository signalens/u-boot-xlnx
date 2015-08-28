/*
 * Copyright (C) 2015 Analog Devices, Inc.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

/*
 * Load and run a standalone elf executable.
 */

#include <common.h>
#include <fs.h>
#include <bootm.h>

int run_elf(char *path, char *addr)
{
	int ret = 0;

	char *load_params[] = {"fatload", "mmc", "0", addr, path};
	char *boot_params[] = {"boote", addr};

	ret = fs_set_blk_dev("mmc", "0", FS_TYPE_FAT);
	if (ret != 0) {
		printf("Failed accessing fat fs on the first MMC partition!\n");
		goto finish;
	}

	ret = do_load(NULL, 0, 5, load_params, FS_TYPE_FAT);
	if (ret != 0) {
		printf("Failed loading %s starting at %s!\n", path, addr);
		goto finish;
	}

	printf("Running %s at %s\n", path, addr);
	ret = do_bootelf(NULL, 0, 2, boot_params);

finish:
	return ret;
}

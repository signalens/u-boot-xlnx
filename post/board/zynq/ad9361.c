/*
 * Copyright (C) 2015 Analog Devices, Inc.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

/*
 * Run AD9361 performance tests.
 */

#include <post.h>
#include <common.h>
#include <fs.h>
#include <bootm.h>

#if CONFIG_POST & CONFIG_SYS_POST_AD9361

#define LOAD_ELF "ad9361.elf"
#define LOAD_ADDR "0x1000000"

DECLARE_GLOBAL_DATA_PTR;

static char *load_params[] = {"fatload", "mmc", "0", LOAD_ADDR, LOAD_ELF};
static char *boot_params[] = {"boote", LOAD_ADDR};

int ad9361_post_test (int flags)
{
	int ret = 0;

	ret = fs_set_blk_dev("mmc", "0", FS_TYPE_FAT);
	if (ret != 0) {
		printf("Failed accessing fat fs on the first MMC partition!\n");
		goto finish;
	}

	ret = do_load(NULL, 0, 5, load_params, FS_TYPE_FAT);
	if (ret != 0) {
		printf("Failed loading %s from fat fs!\n", LOAD_ELF);
		goto finish;
	}

	printf("Running %s\n", LOAD_ELF);
	ret = do_bootelf(NULL, 0, 2, boot_params);

finish:
	return ret;
}

#endif /* CONFIG_POST & CONFIG_SYS_POST_AD9361 */

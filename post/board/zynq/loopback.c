/*
 * Copyright (C) 2015 Analog Devices, Inc.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

/*
 * Loopback test
 *
 * This test checks GPIO functionality by driving signals high and testing the
 * received signals on the matched pins.
 *
 * Note that this test requires using loopback test fixture board.
 */

#include <post.h>
#include "run_elf.h"

#if CONFIG_POST & CONFIG_SYS_POST_LOOPBACK

#define LOAD_ELF "loopback.elf"
#define LOAD_ADDR "0x1000000"

DECLARE_GLOBAL_DATA_PTR;

int loopback_post_test (int flags)
{
	return run_elf(LOAD_ELF, LOAD_ADDR);
}

#endif /* CONFIG_POST & CONFIG_SYS_POST_LOOPBACK */

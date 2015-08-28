/*
 * Copyright (C) 2015 Analog Devices, Inc.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

/*
 * Run AD9361 performance tests.
 */

#include <post.h>
#include "run_elf.h"

#if CONFIG_POST & CONFIG_SYS_POST_AD9361

#define LOAD_ELF "ad9361.elf"
#define LOAD_ADDR "0x1000000"

DECLARE_GLOBAL_DATA_PTR;

int ad9361_post_test (int flags)
{
	return run_elf(LOAD_ELF, LOAD_ADDR);
}

#endif /* CONFIG_POST & CONFIG_SYS_POST_AD9361 */

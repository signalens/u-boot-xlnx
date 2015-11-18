/*
 * Copyright (C) 2015 Analog Devices, Inc.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

/*
 * Ethernet test
 *
 * This test performs a simple ping test for ethernet verification.
 */

#include <post.h>
#include <common.h>
#include <net.h>

#if CONFIG_POST & CONFIG_SYS_POST_ETHER

#define IP_ADDR "192.168.1.99"
#define EXT_ADDR "192.168.1.100"

DECLARE_GLOBAL_DATA_PTR;

int ether_post_test (int flags)
{
	int ret = -1;

	net_ip = string_to_ip(IP_ADDR);
	net_server_ip = string_to_ip(IP_ADDR);
	net_ping_ip = string_to_ip(EXT_ADDR);

	if (net_loop(PING) < 0) {
		printf("ping failed; host %s is not alive\n", EXT_ADDR);
		goto finish;
	}

	ret = 0;

finish:
	return ret;
}

#endif /* CONFIG_POST & CONFIG_SYS_POST_ETHER */

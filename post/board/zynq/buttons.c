/*
 * Copyright (C) 2016 Analog Devices, Inc.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

/*
 * Button and switches test
 *
 * This test verifies button and switch toggling.
 */

#include <post.h>
#include <common.h>
#include <asm/gpio.h>

#if CONFIG_POST & CONFIG_SYS_POST_BUTTONS

// Only one LED and 3 push buttons have pins routed to them on the pzsdr1.
#define LED0 0x39

#define BUTTON1 0x36
#define BUTTON2 0x37
#define BUTTON3 0x38

DECLARE_GLOBAL_DATA_PTR;

int buttons_post_test (int flags)
{
	int ret = -1;
	unsigned long ts;
	int gpios_off = 0;
	int gpios_on = 0;
	const int gpios[] = {BUTTON1, BUTTON2, BUTTON3, -1};
	int orig_gpio_values = gpio_get_values_as_int(gpios);
	int gpio_values;

	printf("\nToggle the buttons (1-3) on the board and watch for corresponding LED state changes.\n");
	printf("The test will time out after 60 seconds if everything hasn't been toggled.\n");

	ts = get_timer(0);

	while ((gpios_off != 0x7) && (gpios_on != 0x7)) {
		gpio_values = gpio_get_values_as_int(gpios);
		gpio_direction_output(LED0, (gpio_values & 0x7));
		if (gpio_values != orig_gpio_values) {
			gpios_off |= ~(gpio_values);
			gpios_on |= gpio_values;
		}
		if (get_timer(ts) > 60000) {
			// timing out
			goto finish;
		}
		mdelay(100);
	}

	ret = 0;

finish:
	return ret;
}

#endif /* CONFIG_POST & CONFIG_SYS_POST_BUTTONS */

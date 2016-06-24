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

#define LED0 0x3a
#define LED1 0x3b
#define LED2 0x3c
#define LED3 0x3d

#define BUTTON0 0x36
#define BUTTON1 0x37
#define BUTTON2 0x38
#define BUTTON3 0x39

#define SWITCH0 0x3e
#define SWITCH1 0x3f
#define SWITCH2 0x40
#define SWITCH3 0x41

DECLARE_GLOBAL_DATA_PTR;

int buttons_post_test (int flags)
{
	int ret = -1;
	unsigned long ts;
	int gpios_off = 0;
	int gpios_on = 0;
	const int gpios[] = {
		BUTTON0, BUTTON1, BUTTON2, BUTTON3,
		SWITCH0, SWITCH1, SWITCH2, SWITCH3, -1};
	int orig_gpio_values = gpio_get_values_as_int(gpios);
	int gpio_values;

	printf("\nToggle the buttons and switches on the board and watch for corresponding LED state changes.\n");
	printf("The test will time out after 60 seconds if everything hasn't been toggled.\n");

	ts = get_timer(0);

	while ((gpios_off != 0xff) && (gpios_on != 0xff)) {
		gpio_values = gpio_get_values_as_int(gpios);
		gpio_direction_output(LED0, (gpio_values & 0x11));
		gpio_direction_output(LED1, (gpio_values & 0x22));
		gpio_direction_output(LED2, (gpio_values & 0x44));
		gpio_direction_output(LED3, (gpio_values & 0x88));
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

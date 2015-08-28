/*
 * Copyright (C) 2015 Analog Devices, Inc.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

/*
 * USB storage test
 *
 * This test performs a read/write test on a connected USB mass storage device.
 * The test passes when data is successfully written to the device, read back,
 * and finally verified.
 */

#include <post.h>
#include <common.h>
#include <mapmem.h>
#include <usb.h>

#if CONFIG_POST & CONFIG_SYS_POST_USB_MEDIA

DECLARE_GLOBAL_DATA_PTR;

#define DATA_BLKS 512
#define START_MEMDATA 0xc100000

int usb_media_post_test (int flags)
{
	int ret = -1, usb_stor_curr_dev = -1, blks = 0, i;
	void *buf_write = map_sysmem(START_MEMDATA, DATA_BLKS);
	void *buf_read = map_sysmem(START_MEMDATA + DATA_BLKS, DATA_BLKS);
	block_dev_desc_t *stor_dev;

	/* TODO: figure out why using aligned buffers hangs on USB read */
	/*ALLOC_CACHE_ALIGN_BUFFER(uchar, buf_write, DATA_BLKS);
	ALLOC_CACHE_ALIGN_BUFFER(uchar, buf_read, DATA_BLKS);*/

	/* initialize memory */
	for (i = 0; i < DATA_BLKS; i++) {
		*((uchar *)(buf_write + i)) = 0xff;
		*((uchar *)(buf_read + i)) = 0x0;
	}

	/* initialize usb */
	if (usb_init() != 0)
		goto finish;

	/* scan for usb storage devices */
	usb_stor_curr_dev = usb_stor_scan(1);
	if (usb_stor_curr_dev != 0) {
		printf("USB: storage device missing!\n");
		goto finish;
	}

	/* write data to device, read it back, and verify it */
	stor_dev = usb_stor_get_dev(usb_stor_curr_dev);
	blks = stor_dev->block_write(usb_stor_curr_dev, 0, DATA_BLKS, buf_write);
	if (blks != DATA_BLKS) {
		printf("USB: error writing data!\n");
		goto finish;
	}
	blks = stor_dev->block_read(usb_stor_curr_dev, 0, DATA_BLKS, buf_read);
	if (blks != DATA_BLKS) {
		printf("USB: error reading data!\n");
		goto finish;
	}
	for (i = 0; i < DATA_BLKS; i++) {
		if (*(uchar *)(buf_read + i) != *(uchar *)(buf_write + i)) {
			printf("USB: data mismatch: %x != %x\n", *(uchar *)(buf_read + i), *(uchar *)(buf_write + i));
			goto finish;
		}
	}

	ret = 0;

finish:
	usb_stop();
	unmap_sysmem(buf_write);
	unmap_sysmem(buf_read);

	return ret;
}

#endif /* CONFIG_POST & CONFIG_SYS_POST_USB_MEDIA */

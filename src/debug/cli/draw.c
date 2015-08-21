/*
 * Commands for testing graphics drawing
 *
 * Copyright (C) 2015 Chromium OS Authors
 */

#include <libpayload.h>
#include <cbfs.h>
#include <coreboot_tables.h>
#include "common.h"
#include "drivers/video/display.h"
#include "drivers/video/coreboot_fb.h"

static int initialized = 0;

static int init_display(void)
{
	if (initialized)
		return 0;
	if (display_init())
		return -1;
	if (backlight_update(1))
		return -1;
	/* initialize video console */
	video_init();
	video_console_clear();
	video_console_cursor_enable(0);

	initialized = 1;
	return 0;
}

static int do_draw_image(const char *name, unsigned long x, unsigned long y)
{
	struct cbfs_file *file;
	file = cbfs_get_file(CBFS_DEFAULT_MEDIA, name);
	if (file == NULL) {
		printf("File '%s' not found\n", name);
		return -1;
	}

	return dc_corebootfb_draw_bitmap(x, y,
					 (void *)file + ntohl(file->offset));
}

static int do_print_info(void)
{
	struct cb_framebuffer *fbinfo = lib_sysinfo.framebuffer;
	if (!fbinfo) {
		printf("Invalid address for framebuffer info\n");
		return -1;
	}

	unsigned char *fbaddr =
		(unsigned char *)(uintptr_t)(fbinfo->physical_address);

	printf("physical_address=0x%p\n", fbaddr);
	printf("x_resolution=%d\n", fbinfo->x_resolution);
	printf("y_resolution=%d\n", fbinfo->y_resolution);
	printf("bytes_per_line=%d\n", fbinfo->bytes_per_line);
	printf("bits_per_pixel=%d\n", fbinfo->bits_per_pixel);
	printf("red_mask_pos=%d\n", fbinfo->red_mask_pos);
	printf("red_mask_size=%d\n", fbinfo->red_mask_size);
	printf("green_mask_pos=%d\n", fbinfo->green_mask_pos);
	printf("green_mask_size=%d\n", fbinfo->green_mask_size);
	printf("blue_mask_pos=%d\n", fbinfo->blue_mask_pos);
	printf("blue_mask_size=%d\n", fbinfo->blue_mask_size);

	return 0;
}

static int do_draw_box(char * const argv[])
{
	const struct vector top_left_rel = {
			.x = strtoul(argv[2], NULL, 0),
			.y = strtoul(argv[3], NULL, 0),
	};
	const struct vector size_rel = {
			.x = strtoul(argv[4], NULL, 0),
			.y = strtoul(argv[5], NULL, 0),
	};
	const struct rgb_color rgb = {
			.red = strtoul(argv[6], NULL, 0),
			.green = strtoul(argv[7], NULL, 0),
			.blue = strtoul(argv[8], NULL, 0),
	};
	return draw_box(&top_left_rel, &size_rel, &rgb);
}

static int do_draw(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	int rv = CMD_RET_SUCCESS;

	if (init_display()) {
		printf("Failed to initialize display\n");
		return CMD_RET_FAILURE;
	}

	if (argc == 5 && !strcmp("image", argv[1])) {
		unsigned long x = strtoul(argv[2], NULL, 10);
		unsigned long y = strtoul(argv[3], NULL, 10);
		/* TODO: validate (x, y) fits in the screen */
		if (do_draw_image(argv[1], x, y))
			rv = CMD_RET_FAILURE;
	} else if (argc == 2 && !strcmp("info", argv[1])) {
		if (do_print_info())
			rv = CMD_RET_FAILURE;
	} else if (argc == 9 && !strcmp("box", argv[1])) {
		if (do_draw_box(argv))
			rv = CMD_RET_FAILURE;
	} else {
		printf("Syntax error\n");
		rv = CMD_RET_USAGE;
	}

	return rv;
}

U_BOOT_CMD(
	   draw,	9,	1,
	   "draw image on screen, print screen info, etc.",
	   "image <name> <x> <y> - load image from cbfs and draw it at (x, y)\n"
	   "info - print framebuffer information\n"
	   "box <x> <y> <width> <height> <red> <green> <blue> - draw a box"
);

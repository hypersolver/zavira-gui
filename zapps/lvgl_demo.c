/* SPDX-License-Identifier: Apache-2.0 */
#include "../include/rpiduino/zapp.h"
#include <zephyr/llext/symbol.h>

static const struct zapp_ui *ui;
static void *progress;
static uint32_t opened_at;
static int started;

void zapp_render(const struct zapp_ui *api, enum zapp_page page)
{
	(void)page;
	ui = api;
	started = 0;
	ui->scene(0x102237);
	ui->panel(22, 24, 596, 400, 0x17314e);
	ui->panel(22, 24, 596, 52, 0x225375);
	ui->panel(22, 75, 596, 2, 0x3ed9e9);
	ui->label(42, 34, "LVGL Demo", 0xf5fbff, 1);
	ui->label(46, 105, "Color and animation", 0xf5fbff, 0);
	ui->panel(46, 145, 160, 64, 0x26bccf);
	ui->panel(230, 145, 160, 64, 0x397fc3);
	ui->panel(414, 145, 160, 64, 0x846ad9);
	progress = ui->bar(46, 244, 528, 16, 0x3ed9e9);
	ui->button(46, 346, 164, 46, "Back to desktop", 0x225375, ZAPP_DESKTOP);
	ui->label(46, 448, "COM3  /  DESKTOP", 0x98c6dc, 0);
}

void zapp_tick(uint32_t now)
{
	if (!started) {
		opened_at = now;
		started = 1;
	}
	ui->set_bar(progress, ((now - opened_at) / 30) % 101);
}

EXPORT_SYMBOL(zapp_render);
EXPORT_SYMBOL(zapp_tick);

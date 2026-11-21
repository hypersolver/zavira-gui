/* SPDX-License-Identifier: Apache-2.0 */
#include "../include/rpiduino/zapp.h"
#include <zephyr/llext/symbol.h>

#define DURATION_MS 10000U
#define TILES 24U

static const struct zapp_ui *ui;
static void *tiles[TILES];
static void *score;
static void *progress;
static uint32_t start;
static uint32_t first_redraw;
static uint32_t previous_second;
static char result[] = "Redraws: 0000    Rate: 000.0/s";
static int running;

static void digits(char *text, unsigned int width, uint32_t value)
{
	while (width) {
		text[--width] = '0' + value % 10;
		value /= 10;
	}
}

void zapp_render(const struct zapp_ui *api, enum zapp_page page)
{
	(void)page;
	ui = api;
	running = 1;
	ui->scene(0x131330);
	ui->label(27, 18, "LVGL Benchmark", 0xf1eaff, 1);
	ui->label(27, 63, "24 animated triangles / 10-second sample", 0xaeb6e9, 0);
	score = ui->label(27, 93, result, 0x69c9ff, 0);
	progress = ui->bar(27, 124, 570, 8, 0x8069ff);
	for (unsigned int i = 0; i < TILES; i++) {
		tiles[i] = ui->triangle(28 + 94 * (i % 6), 151 + 59 * (i / 6), 52,
					i % 2 ? 0x9171ff : 0x54cbfc);
	}
	ui->button(27, 409, 180, 44, "Back to desktop", 0x52409c, ZAPP_DESKTOP);
}

void zapp_tick(uint32_t now)
{
	if (!running) {
		return;
	}
	if (running == 1) {
		start = now;
		first_redraw = ui->redraw_count();
		previous_second = UINT32_MAX;
		running = 2;
	}
	uint32_t elapsed = now - start;
	if (elapsed / 1000 != previous_second) {
		uint32_t frames = ui->redraw_count() - first_redraw;
		uint32_t rate = elapsed ? frames * 10000 / elapsed : 0;

		digits(result + 9, 4, frames);
		digits(result + 23, 3, rate / 10);
		digits(result + 27, 1, rate % 10);
		ui->set_text(score, result);
		ui->set_bar(progress, elapsed / 100);
		previous_second = elapsed / 1000;
	}
	if (elapsed >= DURATION_MS) {
		ui->report_benchmark(ui->redraw_count() - first_redraw, elapsed);
		running = 0;
		return;
	}
	for (unsigned int i = 0; i < TILES; i++) {
		int16_t sweep = (elapsed / 10 + i * 3) % 24;
		struct zapp_point points[3];

		points[0].x = sweep + 1;
		points[0].y = 2;
		points[1].x = 49;
		points[1].y = 14 + sweep;
		points[2].x = 3;
		points[2].y = 48 - sweep;
		ui->set_triangle(tiles[i], points);
	}
}

EXPORT_SYMBOL(zapp_render);
EXPORT_SYMBOL(zapp_tick);

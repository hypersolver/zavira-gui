/* SPDX-License-Identifier: Apache-2.0 */
#include "../include/rpiduino/zapp.h"
#include <zephyr/llext/symbol.h>

static const struct zapp_ui *ui;
static enum zapp_page current_page;
static void *uptime_label;
static void *blades[4];
static void *wind[3];
static void *boot_progress;
static uint32_t boot_started;
static uint32_t last_second;
static char uptime_text[] = "Uptime  00:00:00";

/* Sine scaled by 256 at 15-degree intervals. */
static const int16_t sine[] = {
	0, 66, 128, 181, 222, 247, 256, 247, 222, 181, 128, 66,
	0, -66, -128, -181, -222, -247, -256, -247, -222, -181, -128, -66,
};

static void rotate_pinwheel(unsigned int step)
{
	static const struct zapp_point blade[] = {{5, -8}, {26, -68}, {40, -29}};

	for (unsigned int i = 0; i < 4; i++) {
		unsigned int angle = (step + 6 * i) % 24;
		int s = sine[angle];
		int c = sine[(angle + 6) % 24];
		struct zapp_point points[3];

		for (unsigned int vertex = 0; vertex < 3; vertex++) {
			int x = blade[vertex].x;
			int y = blade[vertex].y;

			points[vertex].x = 80 + (x * c - y * s) / 256;
			points[vertex].y = 80 + (x * s + y * c) / 256;
		}
		ui->set_triangle(blades[i], points);
	}
}

static void accent(int x, int y, int size, uint32_t color,
		   const struct zapp_point points[3])
{
	ui->set_triangle(ui->triangle(x, y, size, color), points);
}

static void animate_wind(unsigned int step)
{
	for (unsigned int i = 0; i < 3; i++) {
		int bend = sine[(step + i * 3) % 24] / 32;
		struct zapp_point points[3];

		points[0].x = 0;
		points[0].y = 32 - bend;
		points[1].x = 120;
		points[1].y = 12 + bend;
		points[2].x = 239;
		points[2].y = 24 - bend;
		ui->set_triangle(wind[i], points);
	}
}

void zapp_render(const struct zapp_ui *api, enum zapp_page page)
{
	ui = api;
	current_page = page;
	ui->scene(0x131330);
	if (page == ZAPP_SPLASH) {
		static const uint32_t colors[] = {0x54cbfc, 0x4189ff, 0x7959f5, 0xad79ff};
		static const struct zapp_point upper[] = {{0, 10}, {10, 0}, {20, 10}};
		static const struct zapp_point lower[] = {{0, 10}, {20, 10}, {10, 20}};
		static const struct zapp_point facet[] = {{0, 119}, {240, 0}, {240, 119}};

		accent(399, 359, 241, 0x242151, facet);
		wind[0] = ui->triangle(0, 186, 240, 0x2b3266);
		wind[1] = ui->triangle(404, 104, 240, 0x312360);
		wind[2] = ui->triangle(399, 127, 240, 0x263e77);
		animate_wind(0);
		for (unsigned int i = 0; i < 4; i++) {
			blades[i] = ui->triangle(240, 83, 160, colors[i]);
		}
		rotate_pinwheel(0);
		accent(310, 153, 22, 0xf0e2ff, upper);
		accent(310, 153, 22, 0x9881ff, lower);
		ui->label(270, 259, "Zavira", 0xf1eaff, 1);
		ui->label(253, 303, "Starting desktop", 0xaeb6e9, 0);
		ui->label(280, 369, ui->version, 0xaeb6e9, 0);
		boot_progress = ui->bar(210, 337, 220, 7, 0x8069ff);
		boot_started = UINT32_MAX;
		return;
	}

	static const struct zapp_point facet[] = {{0, 235}, {235, 0}, {235, 235}};
	static const struct zapp_point overlay[] = {{0, 161}, {161, 0}, {161, 161}};
	accent(404, 196, 236, 0x282454, facet);
	accent(478, 270, 162, 0x352964, overlay);
	ui->panel(0, 0, 640, 66, 0x211d46);
	ui->panel(0, 65, 640, 2, 0x6651cf);
	ui->label(23, 16, "Zavira", 0xf1eaff, 1);
	ui->label(477, 25, "Zephyr", 0xaeb6e9, 0);
	ui->label(540, 25, ui->version, 0x69c9ff, 0);

	ui->panel(24, 97, 338, 235, 0x211d46);
	ui->panel(24, 97, 338, 2, 0x8069ff);
	ui->label(44, 114, "App Launcher", 0xf1eaff, 1);
	ui->button(45, 180, 200, 76, "LVGL Demo", 0x52409c, ZAPP_DEMO);
	ui->button(45, 269, 270, 45, "LVGL Benchmark", 0x52409c, ZAPP_BENCHMARK);

	ui->panel(0, 432, 640, 48, 0x211d46);
	ui->panel(0, 432, 640, 2, 0x6651cf);
	ui->label(23, 448, "COM3  /  115200", 0x69c9ff, 0);
	uptime_label = ui->label(446, 448, uptime_text, 0xf1eaff, 0);
	last_second = UINT32_MAX;
}

static void digits(unsigned int offset, uint32_t value)
{
	uptime_text[offset] = '0' + (value / 10) % 10;
	uptime_text[offset + 1] = '0' + value % 10;
}

void zapp_tick(uint32_t now)
{
	if (current_page == ZAPP_SPLASH) {
		if (boot_started == UINT32_MAX) {
			boot_started = now;
			last_second = UINT32_MAX;
		}
		uint32_t elapsed = now - boot_started;
		uint32_t step = elapsed / 65;

		if (step != last_second) {
			last_second = step;
			rotate_pinwheel(step);
			animate_wind(step);
			ui->set_bar(boot_progress, elapsed / 20);
		}
		return;
	}

	uint32_t seconds = now / 1000;

	if (seconds != last_second) {
		last_second = seconds;
		digits(8, seconds / 3600);
		digits(11, seconds / 60 % 60);
		digits(14, seconds % 60);
		ui->set_text(uptime_label, uptime_text);
	}
}

EXPORT_SYMBOL(zapp_render);
EXPORT_SYMBOL(zapp_tick);

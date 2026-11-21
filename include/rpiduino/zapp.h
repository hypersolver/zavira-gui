#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

enum zapp_page { ZAPP_SPLASH = 1, ZAPP_DESKTOP, ZAPP_DEMO };

struct zapp_point {
    int16_t x;
    int16_t y;
};

struct zapp_ui {
    const char *version;
    void (*scene)(uint32_t color);
    void (*panel)(int x, int y, int width, int height, uint32_t color);
    void *(*label)(int x, int y, const char *text, uint32_t color, int large);
    void (*set_text)(void *label, const char *text);
    void (*button)(int x, int y, int width, int height, const char *text,
                   uint32_t color, enum zapp_page destination);
    void *(*bar)(int x, int y, int width, int height, uint32_t color);
    void (*set_bar)(void *bar, unsigned int percent);
    void *(*triangle)(int x, int y, int size, uint32_t color);
    void (*set_triangle)(void *triangle, const struct zapp_point points[3]);
};

void zapp_request(enum zapp_page page);

#ifdef __cplusplus
}
#endif

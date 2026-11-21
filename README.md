# Zavira GUI

Zavira GUI is an LVGL-based desktop for embedded systems. It provides a purple-blue animated pinwheel splash with moving wind ribbons, the Zavira Launcher, and independent LVGL applications.

| Path | Purpose |
| --- | --- |
| zapps/desktop.c | Animated splash, desktop, and application Launcher |
| zapps/lvgl_demo.c | Demo screen with color swatches and an animated progress bar |
| zapps/lvgl_benchmark.c | Ten-second benchmark with 24 animated triangles |
| include/rpiduino/zapp.h | C interface shared by the modules and their host |

The modules call a host-provided interface for drawing and navigation. The host owns the LVGL instance, display driver, GUI thread, input transport, and module loading. Arduino and XDOS adapters are planned.

## Raspberry Pi 4B hardware result

On Zephyr 4.5.0, the benchmark counted 300 completed LVGL redraws in 10.018 seconds, or **29.95 redraws/s**. This measures the 24-triangle workload with the tested GUI scheduling and display backend. It is not the HDMI refresh rate.

The [Pi 4B integration](https://github.com/hypersolver/Zephry-for-Raspberry-Pi/tree/rpiduino/zavira-gui/docs/rpiduino/zavira-pi4) contains the Zephyr host, COM3 mouse/keyboard bridge, UART controls, and TFTP publishing tools. The [verified benchmark boot package](https://github.com/hypersolver/Zephry-for-Raspberry-Pi/releases/tag/rpiduino-v4.5.0-pi4-zavira-benchmark) includes all three ZAPP modules.

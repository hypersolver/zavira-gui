# Zavira GUI

Zavira GUI is an LVGL-based desktop for embedded systems. The first prototype provides a purple-blue animated pinwheel splash, an application Launcher, and an LVGL Demo with color swatches and an animated progress bar.

| Path | Purpose |
| --- | --- |
| zapps/desktop.c | Splash animation, desktop, and application Launcher |
| zapps/lvgl_demo.c | Demo screen and animated progress bar |
| include/rpiduino/zapp.h | C interface shared between the modules and their host |

The modules call a host-provided UI interface for drawing and navigation. The host owns the LVGL instance, display driver, GUI thread, and module loading. The current prototype is tested with the Zephyr 4.5.0 host. Arduino and XDOS adapters are planned.

The Raspberry Pi integration, build instructions, boot packaging, and hardware validation live in the [rpiduino/zavira-gui branch of Zephry for Raspberry Pi](https://github.com/hypersolver/Zephry-for-Raspberry-Pi/tree/rpiduino/zavira-gui/docs/rpiduino/zavira-pi4). The host pins a revision of this repository and builds desktop.zapp and lvgl-demo.zapp as separate LLEXT modules.

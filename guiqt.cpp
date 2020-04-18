/* xAutoClick, Qt GUI
 *
 * Copyright (C) 2010 Ivo van Poorten
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */

#include <iostream>

#include <QApplication>

#include "clickwidget.h"

extern "C" {
#include "main.h"
//#include "x11clicker.h"
#include "udevclicker.h"
}

//static Display *display;
static udev_clicker_t *ctx;
static QApplication *app;
static ClickWidget *clickWidget;

void click_mouse_button(void) {
//    x11_clicker_click_mouse_button(display);
    udev_clicker_click_mouse_button(ctx);
}

void set_alarm(int ms) {
    clickWidget->startTimer(ms);
}

int get_spin_value(spin_t spin) {
    return clickWidget->getSpinValue(spin);
}

void set_spin_value(spin_t spin, int value) {
    clickWidget->setSpinValue(spin, value);
}

void set_button_sensitive(button_t button, bool state) {
    clickWidget->setButtonSensitive(button, state);
}

int init_gui(int argc, char **argv) {
//    display = x11_clicker_open_display();
    ctx = udev_clicker_init();
    if (/*!display*/!ctx) {
        std::cerr << "udev_clicker_init failed: " << errno << " (" << strerror(errno) << ")\n";
        fprintf(stderr, "Unable to open X display\n");
        return 0;
    }

    static int argn = argc; // QApplication reference to argc workaround
    app = new QApplication(argn, argv);
    clickWidget = new ClickWidget;

    get_options();

    return 1;
}

void close_gui(void) {
    set_options();
//    x11_clicker_close_display(display);
    udev_clicker_destroy(ctx);
}

void main_loop(void) {
    clickWidget->show();
    app->exec();
}

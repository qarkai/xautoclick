/* xAutoClick, Qt 4 GUI
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

#include "clickwidget.h"

extern "C" {
#include "main.h"
}

#include <QApplication>

#include <X11/Xlib.h>
#include <X11/extensions/XTest.h>

static Display *display;
static QApplication *app;
static ClickWidget *clickWidget;

void click_mouse_button(void) {
    XTestFakeButtonEvent(display, 1, True, CurrentTime);
    XTestFakeButtonEvent(display, 1, False, CurrentTime);
    XFlush(display);
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

int init_gui(int /*argc*/, char **/*argv*/) {
    display = XOpenDisplay(NULL);
    if (!display) {
        fprintf(stderr, "Unable to open X display\n");
        return 0;
    }

    app = new QApplication(display);
    clickWidget = new ClickWidget;

    get_options();

    return 1;
}

void close_gui(void) {
    set_options();
    XCloseDisplay(display);
    return;
}

void main_loop(void) {
    clickWidget->show();
    app->exec();
}

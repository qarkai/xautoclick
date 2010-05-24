/* xAutoClick, FLTK GUI
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

extern "C" {
#include "main.h"
#include "osdep.h"
#include <stdio.h>
}

#include <X11/Xlib.h>
#include <X11/extensions/XTest.h>

static Display *display;

void click_mouse_button(void) {
    XTestFakeButtonEvent(display, 1, True, CurrentTime);
    XTestFakeButtonEvent(display, 1, False, CurrentTime);
    XFlush(display);
}

void set_alarm(int ms) {
}

int get_spin_value(spin_t spin) {
//    return mywidget->spins[spin]->value();
}

void set_spin_value(spin_t spin, int value) {
//    mywidget->spins[spin]->setValue(value);
}

void set_button_sensitive(button_t button, int state) {
//    mywidget->buttons[button]->setEnabled(state);
}

int init_gui(int argc, char **argv) {
    if (!(display = XOpenDisplay(NULL))) {
        fprintf(stderr, "Unable to open X display\n");
        return 0;
    }

    return 1;
}

void close_gui(void) {
}

void main_loop(void) {
}

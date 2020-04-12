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

#include <FL/Fl.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Spinner.H>
#include <FL/Fl_Button.H>

extern "C" {
#include "main.h"
#include "osdep.h"
#include <stdio.h>
#include <limits.h>
}

#include <X11/Xlib.h>
#include <X11/extensions/XTest.h>

static Display *display;
static Fl_Double_Window *win;
static Fl_Button *buttons[3];
static Fl_Spinner *spins[4];
static int repeated = 0;

static const char * const label[4] = { "Pre-delay", "Interval", "Random +/-", "# of clicks" };
static const char * const butnames[3] = { "Tap", "Stop", "Start" };

void click_mouse_button(void) {
    XTestFakeButtonEvent(display, 1, True, CurrentTime);
    XTestFakeButtonEvent(display, 1, False, CurrentTime);
    XFlush(display);
}

static void alarm_callback(void *v) {
    common_alarm_callback();
}

void set_alarm(int ms) {
    if (!repeated) {
        Fl::add_timeout(0.001*ms, alarm_callback);
        repeated = 1;
    } else {
        Fl::repeat_timeout(0.001*ms, alarm_callback);
    }
}

int get_spin_value(spin_t spin) {
    return spins[spin]->value();
}

void set_spin_value(spin_t spin, int value) {
    spins[spin]->value(value);
}

void set_button_sensitive(button_t button, bool state) {
    if (state) buttons[button]->activate();
    else       buttons[button]->deactivate();
}

static void tap_callback(Fl_Widget *w, void *v) {
    common_tap_button();
}

static void stop_callback(Fl_Widget *w, void *v) {
    repeated = 0;
    common_stop_button();
}

static void start_callback(Fl_Widget *w, void *v) {
    common_start_button();
}

int init_gui(int argc, char **argv) {
    display = XOpenDisplay(NULL);
    if (!display) {
        fprintf(stderr, "Unable to open X display\n");
        return 0;
    }

    win = new Fl_Double_Window(175, 155, "fltkAutoClick");
    win->begin();
    win->align(FL_ALIGN_CLIP|FL_ALIGN_INSIDE);
    for (int c=0; c<3; c++)
        buttons[c] = new Fl_Button(5+55*c, 125, 55, 25, butnames[c]);
    for (int c=0; c<4; c++) {
        spins[c] = new Fl_Spinner(95, 5+c*30, 75, 25, label[c]);
        spins[c]->minimum(1);
        spins[c]->maximum(INT_MAX);
    }

    buttons[0]->callback(tap_callback);
    buttons[1]->callback(stop_callback);
    buttons[2]->callback(start_callback);
    win->end();

    get_options();

    return 1;
}

void close_gui(void) {
    set_options();
    XCloseDisplay(display);
}

void main_loop(void) {
    win->show();
    Fl::run();
}

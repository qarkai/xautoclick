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
#include "x11clicker.h"
#include <stdio.h>
#include <limits.h>
}

static Display *display;
static Fl_Double_Window *win;
static Fl_Button *buttons[BUTTONS_COUNT];
static Fl_Spinner *spins[SPINS_COUNT];
static bool repeated = false;

void click_mouse_button(void) {
    x11_clicker_click_mouse_button(display);
}

static void alarm_callback(void *v) {
    common_alarm_callback();
}

void set_alarm(int ms) {
    if (!repeated) {
        Fl::add_timeout(0.001*ms, alarm_callback);
        repeated = true;
    } else {
        Fl::repeat_timeout(0.001*ms, alarm_callback);
    }
}

int get_spin_value(spin_t spin) {
    return static_cast<int>(spins[spin]->value());
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
    repeated = false;
    common_stop_button();
}

static void start_callback(Fl_Widget *w, void *v) {
    common_start_button();
}

static Fl_Button* create_button(const char* name, Fl_Callback *callback) {
    static int n = 0;
    const int width = 65;
    auto button = new Fl_Button(5 + width*n, 125, width, 25, name);
    button->callback(callback);
    ++n;
    return button;
}

static Fl_Spinner* create_spin(const char* name) {
    static int n = 0;
    auto spin = new Fl_Spinner(125, 5+n*30, 75, 25, name);
    spin->minimum(1);
    spin->maximum(INT_MAX);
    ++n;
    return spin;
}

int init_gui(int argc, char **argv) {
    display = x11_clicker_open_display();
    if (!display) {
        fprintf(stderr, "Unable to open X display\n");
        return 0;
    }

    win = new Fl_Double_Window(205, 155, "fltkAutoClick");
    win->begin();
    win->align(FL_ALIGN_CLIP|FL_ALIGN_INSIDE);

    buttons[BUTTON_TAP] = create_button("Tap", tap_callback);
    buttons[BUTTON_STOP] = create_button("Stop", stop_callback);
    buttons[BUTTON_START] = create_button("Start", start_callback);

    const char * const label[SPINS_COUNT] = { "Pre-delay, ms", "Interval, ms", "Random +/-, ms", "# of clicks" };
    for (int c = 0; c < SPINS_COUNT; c++)
        spins[c] = create_spin(label[c]);

    win->end();

    get_options();

    return 1;
}

void close_gui(void) {
    set_options();
    x11_clicker_close_display(display);
}

void main_loop(void) {
    win->show();
    Fl::run();
}

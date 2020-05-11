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

#include <climits>
#include <functional>

#include <FL/Fl.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Spinner.H>
#include <FL/Fl_Button.H>

extern "C" {
#include "gui.h"
#include "main.h"
#include "osdep.h"
}

static Fl_Double_Window *win;
static std::array<Fl_Button*, BUTTONS_COUNT> buttons;
static std::array<Fl_Spinner*, SPINS_COUNT> spins;
std::function<void(int)> set_alarm_f;

namespace {

inline void alarm_callback(void*) {
    common_alarm_callback();
}

void set_alarm_init(int ms) {
    Fl::add_timeout(0.001*ms, alarm_callback);
    set_alarm_f = [](int ms) { Fl::repeat_timeout(0.001*ms, alarm_callback); };
}

Fl_Button* create_button(const char* name, Fl_Callback *callback) {
    static int n = 0;
    const int width = 65;
    auto button = new Fl_Button(5 + width*n, 125, width, 25, name);
    button->callback(callback);
    ++n;
    return button;
}

void tap_callback(Fl_Widget*, void*) {
    common_tap_button();
}

void stop_callback(Fl_Widget*, void*) {
    set_alarm_f = set_alarm_init;
    common_stop_button();
}

void start_callback(Fl_Widget*, void*) {
    common_start_button();
}

Fl_Spinner* create_spin(const char* name) {
    static int n = 0;
    auto spin = new Fl_Spinner(125, 5+n*30, 75, 25, name);
    spin->minimum(1);
    spin->maximum(INT_MAX);
    ++n;
    return spin;
}

}

void set_alarm(int ms) {
    set_alarm_f(ms);
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

int init_gui(int, char**) {
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

    set_alarm_f = set_alarm_init;

    get_options();

    return 1;
}

void close_gui(void) {
    set_options();
}

void main_loop(void) {
    win->show();
    Fl::run();
}

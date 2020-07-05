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
#include "gui.h"
#include "main.h"
#include "osdep.h"
}

#include <FL/Fl.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Spinner.H>
#include <FL/Fl_Button.H>

#include <climits>
#include <functional>

struct fltk_gui_t {
    Fl_Double_Window *win;
    std::array<Fl_Spinner*, SPINS_COUNT> spins;
    std::array<Fl_Button*, BUTTONS_COUNT> buttons;
};

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

Fl_Spinner* create_spin(const spin_param_t& param) {
    static int n = 0;
    auto spin = new Fl_Spinner(125, 5+n*30, 75, 25, param.descr);
    spin->range(param.min_value, INT_MAX);

    if (param.suffix && *param.suffix != '\0') {
        auto curFormat = spin->format();
        auto size = strlen(curFormat) + 1 + strlen(param.suffix) + 1;
        char* format = static_cast<char*>(calloc(size, sizeof(char)));
        format = strcat(format, curFormat);
        format = strcat(format, " ");
        format = strcat(format, param.suffix);
        spin->format(format); // TODO: memleak format?
    }

    ++n;
    return spin;
}

void fltk_gui_set_button_sensitive(fltk_gui_t* ctx, button_t button, bool state) {
    if (state) ctx->buttons[button]->activate();
    else       ctx->buttons[button]->deactivate();
}

int fltk_gui_get_spin_value(fltk_gui_t* ctx, spin_t spin) {
    return static_cast<int>(ctx->spins[spin]->value());
}

void fltk_gui_set_spin_value(fltk_gui_t* ctx, spin_t spin, int value) {
    ctx->spins[spin]->value(value);
}

void fltk_gui_main_loop(fltk_gui_t* ctx) {
    ctx->win->show();
    Fl::run();
}

void fltk_gui_close(fltk_gui_t* ctx) {
    delete ctx->win;
    delete ctx;
}

}

void init_gui(gui_t* gui, const spin_param_t* spin_params, int, char**) {
    auto ctx = new fltk_gui_t;

    ctx->win = new Fl_Double_Window(205, 155, "fltkAutoClick");
    ctx->win->begin();
    ctx->win->align(FL_ALIGN_CLIP|FL_ALIGN_INSIDE);

    ctx->buttons[BUTTON_TAP] = create_button("Tap", tap_callback);
    ctx->buttons[BUTTON_STOP] = create_button("Stop", stop_callback);
    ctx->buttons[BUTTON_START] = create_button("Start", start_callback);

    for (int c = 0; c < SPINS_COUNT; c++)
        ctx->spins[c] = create_spin(spin_params[c]);

    ctx->win->end();

    set_alarm_f = set_alarm_init;

    gui->ctx = ctx;
    gui->set_button_sensitive = (gui_set_button_sensitive_t)fltk_gui_set_button_sensitive;
    gui->get_spin_value = (gui_get_spin_value_t)fltk_gui_get_spin_value;
    gui->set_spin_value = (gui_set_spin_value_t)fltk_gui_set_spin_value;
    gui->main_loop = (gui_main_loop_t)fltk_gui_main_loop;
    gui->close = (gui_close_t)fltk_gui_close;
}

void set_alarm(int ms) {
    set_alarm_f(ms);
}

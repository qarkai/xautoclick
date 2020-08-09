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
#include <FL/Fl_Button.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Spinner.H>

#include <algorithm>
#include <climits>
#include <functional>

struct fltk_gui_t {
    Fl_Double_Window *win;
    std::array<Fl_Spinner*, SPINS_COUNT> spins;
    std::array<Fl_Button*, BUTTONS_COUNT> buttons;
};

std::function<void(int)> set_alarm_f;

static constexpr int border = 10;
static constexpr int space = 5;
static constexpr int spin_width = 75;
static constexpr int btn_width = 55;
static constexpr int height = 25;

namespace {

inline void alarm_callback(void*) {
    common_alarm_callback();
}

void set_alarm_init(int ms) {
    Fl::add_timeout(0.001*ms, alarm_callback);
    set_alarm_f = [](int ms) { Fl::repeat_timeout(0.001*ms, alarm_callback); };
}

Fl_Button* create_button(int win_height, const char** button_names, const std::array<Fl_Callback*, BUTTONS_COUNT>& callbacks, int n) {
    auto button = new Fl_Button(border + (btn_width + space) * n, win_height - (height + border), btn_width, height, button_names[n]);
    button->callback(callbacks[n]);
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

Fl_Spinner* create_spin(int win_width, const spin_param_t* spin_params, int n) {
    const auto& param = spin_params[n];
    auto spin = new Fl_Spinner(win_width - (spin_width + border), border + n * (space + height), spin_width, height, param.descr);
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

void init_gui(gui_t* gui, const spin_param_t* spin_params, const char** button_names, int, char**) {
    auto ctx = new fltk_gui_t;

    auto buttons_width = btn_width * BUTTONS_COUNT + space * (BUTTONS_COUNT - 1);
    auto win_width = std::max(/*label_width*/90 + spin_width, buttons_width) + border * 2;

    auto row_count = SPINS_COUNT + 1;
    auto win_height = (height + space) * row_count + border * 2;

    ctx->win = new Fl_Double_Window(win_width, win_height, "fltkAutoClick");
    ctx->win->begin();
    ctx->win->align(FL_ALIGN_CLIP|FL_ALIGN_INSIDE);

    for (int c = 0; c < SPINS_COUNT; c++)
        ctx->spins[c] = create_spin(win_width, spin_params, c);

    std::array<Fl_Callback*, BUTTONS_COUNT> callbacks = { tap_callback, stop_callback, start_callback };
    for (int c = 0; c < BUTTONS_COUNT; c++)
        ctx->buttons[c] = create_button(win_height, button_names, callbacks, c);

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

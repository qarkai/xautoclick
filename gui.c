/*
 * xAutoClick
 *
 * Copyright (C) 2020 Arkadiy Illarionov
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

#include "gui.h"

#include <stdlib.h>

gui_t* gui_init(int argc, char **argv) {
    const spin_param_t spin_params[SPINS_COUNT] = {
        {"Pre-delay", "ms", 0},
        {"Interval", "ms", 0},
        {"Random +/-", "ms", 0},
        {"# of clicks", "", 1}
    };
    const char* button_names[BUTTONS_COUNT] = { "Tap", "Stop", "Start" };
    gui_t* gui;

    gui = calloc(1, sizeof(gui_t));
    if (!gui)
        return NULL;

    gui->is_save_values = true;

    init_gui(gui, spin_params, button_names, argc, argv);
    if (!gui->ctx) {
        free(gui);
        return NULL;
    }

    return gui;
}

void gui_set_button_sensitive(gui_t* gui, button_t button, bool state) {
    if (gui && gui->set_button_sensitive)
        gui->set_button_sensitive(gui->ctx, button, state);
}

int gui_get_spin_value(const gui_t* gui, spin_t spin) {
    if (gui && gui->get_spin_value)
        return gui->get_spin_value(gui->ctx, spin);
    return -1;
}

void gui_set_spin_value(gui_t* gui, spin_t spin, int value) {
    if (gui && gui->set_spin_value)
        gui->set_spin_value(gui->ctx, spin, value);
}

void gui_main_loop(gui_t* gui) {
    if (gui && gui->main_loop)
        gui->main_loop(gui->ctx);
}

void gui_close(gui_t* gui) {
    if (gui && gui->close)
        gui->close(gui->ctx);

    free(gui);
}

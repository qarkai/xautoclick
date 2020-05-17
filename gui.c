#include "gui.h"

#include <stdlib.h>

gui_t* gui_init(int argc, char **argv) {
    gui_t* gui;

    gui = calloc(1, sizeof(gui_t));
    if (!gui)
        return NULL;

    init_gui(gui, argc, argv);
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

int gui_get_spin_value(gui_t* gui, spin_t spin) {
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

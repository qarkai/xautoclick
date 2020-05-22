#ifndef __GUI_H__
#define __GUI_H__

#include <stdbool.h>

typedef enum button_e {
    BUTTON_TAP = 0,
    BUTTON_STOP,
    BUTTON_START,
    BUTTONS_COUNT
} button_t;

typedef enum spin_e {
    SPIN_PREDELAY = 0,
    SPIN_INTERVAL,
    SPIN_RANDOM,
    SPIN_NUMBER,
    SPINS_COUNT
} spin_t;

typedef void (*gui_set_button_sensitive_t)(void* ctx, button_t button, bool state);
typedef int (*gui_get_spin_value_t)(void* ctx, spin_t spin);
typedef void (*gui_set_spin_value_t)(void* ctx, spin_t spin, int value);
typedef void (*gui_main_loop_t)(void* ctx);
typedef void (*gui_close_t)(void* ctx);

typedef struct gui_ctx {
    void* ctx;
    bool is_save_values;

    gui_set_button_sensitive_t set_button_sensitive;
    gui_get_spin_value_t get_spin_value;
    gui_set_spin_value_t set_spin_value;
    gui_main_loop_t main_loop;
    gui_close_t close;
} gui_t;

gui_t* gui_init(int argc, char **argv);
void gui_set_button_sensitive(gui_t* gui, button_t button, bool state);
int gui_get_spin_value(const gui_t* gui, spin_t spin);
void gui_set_spin_value(gui_t* gui, spin_t spin, int value);
void gui_main_loop(gui_t* gui);
void gui_close(gui_t* gui);

/* Stuff that has to be implemented for each GUI */

void init_gui(gui_t* gui, int argc, char **argv);
void set_alarm(int ms);

#endif

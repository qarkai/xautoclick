#ifndef __MAIN_H__
#define __MAIN_H__

#include <stdbool.h>

typedef enum button_e {
    BUTTON_TAP = 0,
    BUTTON_STOP,
    BUTTON_START
} button_t;

typedef enum spin_e {
    SPIN_PREDELAY = 0,
    SPIN_INTERVAL,
    SPIN_RANDOM,
    SPIN_NUMBER
} spin_t;

/* Common functions. GUI's should call back to these */

void common_tap_button(void);
void common_stop_button(void);
void common_start_button(void);

void common_alarm_callback(void);

/* Stuff that has to be implemented for each GUI */

int init_gui(int argc, char **argv);
void main_loop(void);
void close_gui(void);
void set_button_sensitive(button_t button, bool state);
void set_spin_value(spin_t spin, int value);
int get_spin_value(spin_t spin);
void get_options(void);
void set_options(void);
void set_alarm(int ms);
void click_mouse_button(void);

#endif

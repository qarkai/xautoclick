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

/* Stuff that has to be implemented for each GUI */

int init_gui(int argc, char **argv);
void set_button_sensitive(button_t button, bool state);
int get_spin_value(spin_t spin);
void set_spin_value(spin_t spin, int value);
void main_loop(void);
void set_alarm(int ms);
void close_gui(void);

#endif

#ifndef __MAIN_H__
#define __MAIN_H__

typedef struct gui_ctx gui_t;

/* Common functions. GUI's should call back to these */

void get_options(gui_t *gui);
void set_options(void);

void common_tap_button(void);
void common_stop_button(void);
void common_start_button(void);

void common_alarm_callback(void);

#endif

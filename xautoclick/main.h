/* ------------------------------------------------------------------------- */

/*
 * $Id$
 * 
 * xAutoClick
 *
 * Copyright (C) 2006 Ivo van Poorten
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

/* ------------------------------------------------------------------------- */

#ifndef __MAIN_H__
#define __MAIN_H_

/* ------------------------------------------------------------------------- */

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

/* ------------------------------------------------------------------------- */

/* Common functions. GUI's should call back to these */

void common_tap_button(void);
void common_stop_button(void);
void common_start_button(void);

void common_alarm_callback(void);

/* ------------------------------------------------------------------------- */

/* Stuff that has to be implemented for each GUI */

int init_gui(int argc, char **argv);
void close_gui(void);
void main_loop(void);
void set_button_sensitive(button_t button, int state);
void set_spin_value(spin_t spin, int value);
int get_spin_value(spin_t spin);
void set_alarm(int ms);
void click_mouse_button(void);

/* ------------------------------------------------------------------------- */

#endif

/* ------------------------------------------------------------------------- */


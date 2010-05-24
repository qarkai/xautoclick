/*
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

#include <stdio.h>
#include <stdlib.h>

#include "main.h"
#include "osdep.h"

static int counter = 0;

void common_stop_button(void) {

    set_button_sensitive(BUTTON_TAP, 1);
    set_button_sensitive(BUTTON_STOP, 0);
    set_button_sensitive(BUTTON_START, 1);

    counter = 0;
}

void common_start_button(void) {

    set_button_sensitive(BUTTON_TAP, 0);
    set_button_sensitive(BUTTON_STOP, 1);
    set_button_sensitive(BUTTON_START, 0);

    counter = get_spin_value(SPIN_NUMBER);

    set_alarm(get_spin_value(SPIN_PREDELAY));
}

void common_alarm_callback(void) {
    int interval, randomfactor, sign, rv, alarmtime;

    if (!counter) return;

#ifdef DEBUG
    printf("alarm_callback\n");
#endif

    click_mouse_button();

    interval     = get_spin_value(SPIN_INTERVAL);
    sign         = rand() / (RAND_MAX >> 1);
    randomfactor = get_spin_value(SPIN_RANDOM);

    if (randomfactor > 0 )
        rv = (sign ? 1 : -1) * (rand() / (RAND_MAX / randomfactor));
    else
        rv = 0;

#ifdef DEBUG
    printf("rv = %i\n", rv);
#endif

    alarmtime = interval + rv;
    if (alarmtime < 1) alarmtime = 1;

    counter--;
    if (counter) set_alarm(alarmtime);
    else         common_stop_button();
}

static void calculate_average(int *buffer, int length, int *average, int *min,
                                                                int *max) {
    int sum = 0, x, v;
    
    *min =  65536;
    *max = -65536;

    for (x=0; x<length; x++) {
        v = buffer[x];
        sum += v;
        if (v<*min) *min=v;
        if (v>*max) *max=v;
    }

    *average = sum/length;

#ifdef DEBUG
    printf("average = %i // min = %i // max = %i  (of %i samples)\n",
                                                *average, *min, *max, length);
#endif
}

#define THRESHOLD 5 * 1000     /* 5 seconds */
#define HISTORYSIZE 10

void common_tap_button(void) {
    static int history[HISTORYSIZE];
    static int prevtime = 0, x = 0, fill = 0, curtime, interval;
    int average, min, max;

    curtime  = GetTimer() / 1000;
    interval = curtime - prevtime;

#ifdef DEBUG
    printf("interval = %i\n", interval);
#endif

    if ( interval > THRESHOLD ) {
#ifdef DEBUG
        printf("new tapping...\n");
#endif
        prevtime = curtime;
        x = fill = 0;
        return;
    }

    history[x] = interval;
    x++;
    fill++;

    if (x == HISTORYSIZE)       x = 0;
    if (fill > HISTORYSIZE)     fill = HISTORYSIZE;

    calculate_average(history, fill, &average, &min, &max);

    set_spin_value(SPIN_INTERVAL, average);
    set_spin_value(SPIN_RANDOM, (max-min)>>1);
    prevtime = curtime;
}

int main(int argc, char **argv) {

    if (!init_gui(argc, argv)) {
        fprintf(stderr, "Unable to initialize GUI\n");
        return -1;
    }

    set_button_sensitive(BUTTON_TAP, 1);
    set_button_sensitive(BUTTON_STOP, 0);
    set_button_sensitive(BUTTON_START, 1);

    main_loop();
    close_gui();
    return 0;
}

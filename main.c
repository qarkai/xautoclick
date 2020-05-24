/*
 * xAutoClick
 *
 * Copyright (C) 2006-2012 Ivo van Poorten
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

#include "clicker.h"
#include "gui.h"
#include "main.h"
#include "options.h"
#include "osdep.h"

static clicker_t *clicker;
static gui_t *gui;
static int counter = 0;
static int interval = 0;
static int randomfactor = 0;

static void reset_buttons_state(gui_t *gui) {
    gui_set_button_sensitive(gui, BUTTON_TAP, true);
    gui_set_button_sensitive(gui, BUTTON_STOP, false);
    gui_set_button_sensitive(gui, BUTTON_START, true);
}

void common_stop_button(void) {
    reset_buttons_state(gui);
    counter = 0;
}

void common_start_button(void) {
    gui_set_button_sensitive(gui, BUTTON_TAP, false);
    gui_set_button_sensitive(gui, BUTTON_STOP, true);
    gui_set_button_sensitive(gui, BUTTON_START, false);

    counter = gui_get_spin_value(gui, SPIN_NUMBER);
    interval  = gui_get_spin_value(gui, SPIN_INTERVAL);
    randomfactor = gui_get_spin_value(gui, SPIN_RANDOM);
    set_alarm(gui_get_spin_value(gui, SPIN_PREDELAY));
}

void common_alarm_callback(void) {
    int alarmtime;
    int rv = 0;

    if (!counter)
        return;

#ifdef DEBUG
    printf("alarm_callback\n");
#endif

    clicker_click(clicker);

    if (randomfactor > 0)
    {
        int sign = rand() / (RAND_MAX >> 1);

        rv = (sign ? 1 : -1) * (rand() / (RAND_MAX / randomfactor));
    }

#ifdef DEBUG
    printf("rv = %i\n", rv);
#endif

    alarmtime = interval + rv;
    if (alarmtime < 1)
        alarmtime = 1;

    --counter;
    if (counter)
        set_alarm(alarmtime);
    else
        common_stop_button();
}

static int calculate_average(const int *buffer, int length, int *min, int *max) {
    int sum = 0;
    int average;

    *min =  65536;
    *max = -65536;

    for (int x = 0; x < length; ++x) {
        int v = buffer[x];
        sum += v;

        if (v < *min) {
            *min = v;
        }

        if (v > *max) {
            *max = v;
        }
    }

    average = sum/length;

#ifdef DEBUG
    printf("average = %i // min = %i // max = %i (of %i samples)\n",
           average, *min, *max, length);
#endif

    return average;
}

#define THRESHOLD (5 * 1000)   /* 5 seconds */
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
    ++x;
    ++fill;

    if (x == HISTORYSIZE) {
        x = 0;
    }

    if (fill > HISTORYSIZE) {
        fill = HISTORYSIZE;
    }

    average = calculate_average(history, fill, &min, &max);

    gui_set_spin_value(gui, SPIN_INTERVAL, average);
    gui_set_spin_value(gui, SPIN_RANDOM, (max - min) >> 1);
    prevtime = curtime;
}

static void get_options(gui_t *gui, const options_t *options) {
    gui_set_spin_value(gui, SPIN_PREDELAY, options->predelay);
    gui_set_spin_value(gui, SPIN_INTERVAL, options->interval);
    gui_set_spin_value(gui, SPIN_RANDOM, options->random_factor);
    gui_set_spin_value(gui, SPIN_NUMBER, options->clicks_number);
}

static void set_options(const gui_t *gui, options_t *options) {
    options->predelay = gui_get_spin_value(gui, SPIN_PREDELAY);
    options->interval = gui_get_spin_value(gui, SPIN_INTERVAL);
    options->random_factor = gui_get_spin_value(gui, SPIN_RANDOM);
    options->clicks_number = gui_get_spin_value(gui, SPIN_NUMBER);
}

int main(int argc, char **argv) {
    options_t options = {
        .predelay = 2000,
        .interval = 1000,
        .random_factor = 250,
        .clicks_number = 100
    };
    const char* config_file = get_config_file();

    load_options(config_file, &options);

    clicker = clicker_init();
    if (!clicker) {
        fprintf(stderr, "Unable to initialize clicker\n");
        return -1;
    }

    gui = gui_init(argc, argv);
    if (!gui) {
        fprintf(stderr, "Unable to initialize GUI\n");
        clicker_close(clicker);
        return -1;
    }

    get_options(gui, &options);
    reset_buttons_state(gui);

    gui_main_loop(gui);

    if (gui->is_save_values) {
        set_options(gui, &options);
        save_options(config_file, &options);
    }

    gui_close(gui);
    clicker_close(clicker);

    return 0;
}

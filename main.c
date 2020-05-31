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
#include <time.h>

#include "clicker.h"
#include "gui.h"
#include "main.h"
#include "options.h"
#include "osdep.h"

typedef int (*get_alarm_time_t)(int interval, int random_factor);

typedef struct {
    options_t options;
    clicker_t *clicker;
    gui_t *gui;
    get_alarm_time_t get_alarm_time;
    int counter;
} main_t;

static main_t main_ctx = { OPTIONS_INIT, NULL, NULL, NULL, 0 };

static options_t get_values(const gui_t *gui) {
    options_t options = {
        .predelay      = gui_get_spin_value(gui, SPIN_PREDELAY),
        .interval      = gui_get_spin_value(gui, SPIN_INTERVAL),
        .random_factor = gui_get_spin_value(gui, SPIN_RANDOM),
        .clicks_number = gui_get_spin_value(gui, SPIN_NUMBER)
    };

    return options;
}

static void set_values(gui_t *gui, const options_t *options) {
    gui_set_spin_value(gui, SPIN_PREDELAY, options->predelay);
    gui_set_spin_value(gui, SPIN_INTERVAL, options->interval);
    gui_set_spin_value(gui, SPIN_RANDOM, options->random_factor);
    gui_set_spin_value(gui, SPIN_NUMBER, options->clicks_number);
}

static void reset_buttons_state(gui_t *gui) {
    gui_set_button_sensitive(gui, BUTTON_TAP, true);
    gui_set_button_sensitive(gui, BUTTON_STOP, false);
    gui_set_button_sensitive(gui, BUTTON_START, true);
}

static int get_alarm_time_simple(int interval, int random_factor) {
    (void)random_factor;
    return interval;
}

static int get_alarm_time_full(int interval, int random_factor) {
    int sign = rand() / (RAND_MAX >> 1);
    int random_value = (sign ? 1 : -1) * (rand() / (RAND_MAX / random_factor));
#ifdef DEBUG
    fprintf(stdout, "random_value = %i\n", random_value);
#endif
    return interval + random_value;
}

void common_stop_button(void) {
    reset_buttons_state(main_ctx.gui);
    main_ctx.counter = 0;
}

void common_start_button(void) {
    gui_set_button_sensitive(main_ctx.gui, BUTTON_TAP, false);
    gui_set_button_sensitive(main_ctx.gui, BUTTON_STOP, true);
    gui_set_button_sensitive(main_ctx.gui, BUTTON_START, false);

    main_ctx.options = get_values(main_ctx.gui);

    main_ctx.get_alarm_time = (main_ctx.options.random_factor > 0) ? get_alarm_time_full : get_alarm_time_simple;
    main_ctx.counter = main_ctx.options.clicks_number;
    set_alarm(main_ctx.options.predelay);
}

void common_alarm_callback(void) {
    int alarmtime;

    if (!main_ctx.counter)
        return;

#ifdef DEBUG
    printf("alarm_callback\n");
#endif

    clicker_click(main_ctx.clicker);

    alarmtime = main_ctx.get_alarm_time(main_ctx.options.interval, main_ctx.options.random_factor);
    if (alarmtime < 1)
        alarmtime = 1;

    --main_ctx.counter;
    if (main_ctx.counter)
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

static unsigned int get_timer(void) {
    struct timespec tv;

    timespec_get(&tv, TIME_UTC);

    return tv.tv_sec * 1000000 + tv.tv_nsec / 1000;
}

void common_tap_button(void) {
    static int history[HISTORYSIZE];
    static int prevtime = 0, x = 0, fill = 0;
    int curtime, interval;
    int min, max;

    curtime  = get_timer() / 1000;
    interval = curtime - prevtime;

#ifdef DEBUG
    printf("interval = %i\n", interval);
#endif

    if (interval > THRESHOLD) {
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

    main_ctx.options.interval = calculate_average(history, fill, &min, &max);
    main_ctx.options.random_factor = (max - min) >> 1;

    set_values(main_ctx.gui, &main_ctx.options);

    prevtime = curtime;
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

    main_ctx.options = options;

    main_ctx.clicker = clicker_init();
    if (!main_ctx.clicker) {
        fprintf(stderr, "Unable to initialize clicker\n");
        return -1;
    }

    main_ctx.gui = gui_init(argc, argv);
    if (!main_ctx.gui) {
        fprintf(stderr, "Unable to initialize GUI\n");
        clicker_close(main_ctx.clicker);
        return -1;
    }

    set_values(main_ctx.gui, &main_ctx.options);
    reset_buttons_state(main_ctx.gui);

    gui_main_loop(main_ctx.gui);

    if (main_ctx.gui->is_save_values) {
        options = get_values(main_ctx.gui);
        save_options(config_file, &options);
    }

    gui_close(main_ctx.gui);
    clicker_close(main_ctx.clicker);

    return 0;
}

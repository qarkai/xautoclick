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
#include <string.h>
#include <errno.h>
#include <error.h>
#include <sys/stat.h>

#include "main.h"
#include "osdep.h"

typedef struct options {
    char *config_file;
    int predelay;
    int interval;
    int random_factor;
    int clicks_number;
} options_t;

static options_t options = {
    .config_file = NULL,
    .predelay = 2000,
    .interval = 1000,
    .random_factor = 250,
    .clicks_number = 100
};

static int counter = 0;

void common_stop_button(void) {

    set_button_sensitive(BUTTON_TAP, true);
    set_button_sensitive(BUTTON_STOP, false);
    set_button_sensitive(BUTTON_START, true);

    counter = 0;
}

void common_start_button(void) {

    set_button_sensitive(BUTTON_TAP, false);
    set_button_sensitive(BUTTON_STOP, true);
    set_button_sensitive(BUTTON_START, false);

    counter = get_spin_value(SPIN_NUMBER);

    set_alarm(get_spin_value(SPIN_PREDELAY));
}

void common_alarm_callback(void) {
    int interval, randomfactor, alarmtime;
    int rv = 0;

    if (!counter)
        return;

#ifdef DEBUG
    printf("alarm_callback\n");
#endif

    click_mouse_button();

    interval     = get_spin_value(SPIN_INTERVAL);
    randomfactor = get_spin_value(SPIN_RANDOM);

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
    int sum = 0, x;
    int average;

    *min =  65536;
    *max = -65536;

    for (x = 0; x < length; ++x) {
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

    set_spin_value(SPIN_INTERVAL, average);
    set_spin_value(SPIN_RANDOM, (max - min) >> 1);
    prevtime = curtime;
}

void get_options(void)
{
    set_spin_value(SPIN_PREDELAY, options.predelay);
    set_spin_value(SPIN_INTERVAL, options.interval);
    set_spin_value(SPIN_RANDOM, options.random_factor);
    set_spin_value(SPIN_NUMBER, options.clicks_number);
}

void set_options(void)
{
    options.predelay = get_spin_value(SPIN_PREDELAY);
    options.interval = get_spin_value(SPIN_INTERVAL);
    options.random_factor = get_spin_value(SPIN_RANDOM);
    options.clicks_number = get_spin_value(SPIN_NUMBER);
}

static int read_option(FILE *cfg_file, char *name, int *value)
{
    int result;
    char scan_format[20] = {0};

    sprintf(scan_format, "%s=%%d\n", name);

    result = fscanf(cfg_file, scan_format, value);
    if (result != 1)
    {
        fprintf(stderr, "Can't read %s value", name);

        if (EOF == result)
        {
            int stream_err = ferror(cfg_file);

            if (stream_err != 0)
            {
                fprintf(stderr, ", error: %d (%s)", errno, strerror(errno));
            }
            else
            {
                fprintf(stderr, ", result: %d (Reached end of input)", result);
            }
        }

        fprintf(stderr, "\n");
    }

    return result;
}

static void load_config(options_t *opts)
{
    struct config_options_struct {
        char *name;
        int *value;
    } config_options[4] = {
        { .name = "predelay", .value = &opts->predelay},
        { .name = "interval", .value = &opts->interval},
        { .name = "random_factor", .value = &opts->random_factor},
        { .name = "clicks_number", .value = &opts->clicks_number}
    };
    struct stat config_dir_stat = {0};
    FILE *config_file = NULL;
    int result;
    int i;
    char *env_config_dir;
    char *config_dir;

    env_config_dir = getenv("XDG_CONFIG_HOME");
    if (NULL == env_config_dir || '\0' == env_config_dir[0])
    {
        env_config_dir = getenv("HOME");
        config_dir = calloc(strlen(env_config_dir) + strlen("/.config/xautoclick") + 1, sizeof(char));
        sprintf(config_dir, "%s/.config/xautoclick", env_config_dir);
    }
    else
    {
        config_dir = calloc(strlen(env_config_dir) + strlen("/xautoclick") + 1, sizeof(char));
        sprintf(config_dir, "%s/xautoclick", env_config_dir);
    }

    result = stat(config_dir, &config_dir_stat);
    if (result == -1)
    {
        /* create xautoclick config dir */
        mkdir(config_dir, 0700);
    }

    opts->config_file = calloc(strlen(config_dir) + strlen("/config") + 1, sizeof(char));
    sprintf(opts->config_file, "%s/config", config_dir);
    free(config_dir);

    config_file = fopen(opts->config_file, "r");
    if (NULL == config_file)
    {
        error(0, errno, "Can't open config file %s", opts->config_file);
        return;
    }

    for (i = 0; i < 4; ++i)
    {
        result = read_option(config_file, config_options[i].name, config_options[i].value);
        if (result != 1)
        {
            fprintf(stderr, "Can't parse config file %s", opts->config_file);
        }
    }

#ifdef DEBUG
    printf("%d, %d, %d, %d\n", options->predelay, options->interval, options->random_factor, options->clicks_number);
#endif

    fclose(config_file);

    return;
}

static void save_config(options_t *opts)
{
    FILE *config_file = NULL;

    config_file = fopen(opts->config_file, "w+");
    if (!config_file)
      return;
    fprintf(config_file, "predelay=%d\n", opts->predelay);
    fprintf(config_file, "interval=%d\n", opts->interval);
    fprintf(config_file, "random_factor=%d\n", opts->random_factor);
    fprintf(config_file, "clicks_number=%d\n", opts->clicks_number);
    fclose(config_file);
}

int main(int argc, char **argv) {
    load_config(&options);

    if (!init_gui(argc, argv)) {
        fprintf(stderr, "Unable to initialize GUI\n");
        return -1;
    }

    set_button_sensitive(BUTTON_TAP, true);
    set_button_sensitive(BUTTON_STOP, false);
    set_button_sensitive(BUTTON_START, true);

    main_loop();
    close_gui();

    save_config(&options);

    return 0;
}

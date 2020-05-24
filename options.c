/*
 * xAutoClick
 *
 * Copyright (C) 2020 Arkadiy Illarionov
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

#include "options.h"

#include <sys/stat.h>
#include <error.h>

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* get_config_file(void) {
    struct stat config_dir_stat = {0};
    int result;
    char *env_config_dir;
    char *config_dir;
    char *config_file = NULL;

    env_config_dir = getenv("XDG_CONFIG_HOME");
    if (!env_config_dir || *env_config_dir == '\0') {
        env_config_dir = getenv("HOME");
        config_dir = calloc(strlen(env_config_dir) + strlen("/.config/xautoclick") + 1, sizeof(char));
        sprintf(config_dir, "%s/.config/xautoclick", env_config_dir);
    }
    else {
        config_dir = calloc(strlen(env_config_dir) + strlen("/xautoclick") + 1, sizeof(char));
        sprintf(config_dir, "%s/xautoclick", env_config_dir);
    }

    result = stat(config_dir, &config_dir_stat);
    if (result == -1) {
        /* create xautoclick config dir */
        mkdir(config_dir, 0700);
    }

    config_file = calloc(strlen(config_dir) + strlen("/config") + 1, sizeof(char));
    sprintf(config_file, "%s/config", config_dir);
    free(config_dir);

    return config_file;
}

static int read_option(FILE *cfg_file, const char *name, int *value) {
    int result;
    char scan_format[20] = {0};

    sprintf(scan_format, "%s=%%d\n", name);

    result = fscanf(cfg_file, scan_format, value);
    if (result != 1) {
        fprintf(stderr, "Can't read %s value", name);

        if (result == EOF) {
            int stream_err = ferror(cfg_file);

            if (stream_err != 0)
                fprintf(stderr, ", error: %d (%s)", errno, strerror(errno));
            else
                fprintf(stderr, ", result: %d (Reached end of input)", result);
        }

        fprintf(stderr, "\n");
    }

    return result;
}

void load_options(const char* filename, options_t *opts) {
    struct config_option {
        char *name;
        int *value;
    } config_options[] = {
        { .name = "predelay", .value = &opts->predelay},
        { .name = "interval", .value = &opts->interval},
        { .name = "random_factor", .value = &opts->random_factor},
        { .name = "clicks_number", .value = &opts->clicks_number}
    };
    size_t config_opts_num = sizeof(config_options) / sizeof(struct config_option);
    FILE *config_file = NULL;

    config_file = fopen(filename, "r");
    if (!config_file) {
        error(0, errno, "Can't open config file %s", filename);
        return;
    }

    for (size_t i = 0; i < config_opts_num; ++i) {
        int read_count = read_option(config_file, config_options[i].name, config_options[i].value);
        if (read_count != 1)
            fprintf(stderr, "Can't parse config file %s\n", filename);
    }

#ifdef DEBUG
    printf("%d, %d, %d, %d\n", options->predelay, options->interval, options->random_factor, options->clicks_number);
#endif

    fclose(config_file);

    return;
}

void save_options(const char* filename, const options_t *opts) {
    FILE *config_file;

    config_file = fopen(filename, "w+");
    if (!config_file)
        return;

    fprintf(config_file, "predelay=%d\n", opts->predelay);
    fprintf(config_file, "interval=%d\n", opts->interval);
    fprintf(config_file, "random_factor=%d\n", opts->random_factor);
    fprintf(config_file, "clicks_number=%d\n", opts->clicks_number);
    fclose(config_file);
}

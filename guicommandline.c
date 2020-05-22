/* xAutoClick -- Command Line "GUI"
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
#include <string.h>

#include <getopt.h>

#include "gui.h"
#include "main.h"
#include "osdep.h"

typedef struct cli_gui_ctx {
    int argc;
    char **argv;
    int spins[SPINS_COUNT];
} cli_gui_t;

static int sleeptime;

static void printhelp(char *myname);
static int cli_gui_get_spin_value(cli_gui_t* ctx, spin_t spin);
static void cli_gui_set_spin_value(cli_gui_t* ctx, spin_t spin, int value);
static void cli_gui_main_loop(cli_gui_t* ctx);
static void cli_gui_close(cli_gui_t* ctx);

void init_gui(gui_t* gui, int argc, char **argv) {
    cli_gui_t* ctx;

    ctx = calloc(1, sizeof(cli_gui_t));
    if (!ctx) {
        fprintf(stderr, "Can't allocate memory for CLI GUI\n");
        return;
    }

    ctx->argc = argc;
    ctx->argv = argv;

    gui->ctx = ctx;
    gui->is_save_values = false; /* don't set options from command line */
    gui->get_spin_value = (gui_get_spin_value_t)cli_gui_get_spin_value;
    gui->set_spin_value = (gui_set_spin_value_t)cli_gui_set_spin_value;
    gui->main_loop = (gui_main_loop_t)cli_gui_main_loop;
    gui->close = (gui_close_t)cli_gui_close;
}

void set_alarm(int ms) {
    sleeptime = ms;
}

static void printhelp(char *myname) {
    printf("usage: %s [-h][-i <interval, ms>][-n <# of clicks>][-p <pre-delay, ms>][-r <random +/-, ms>]\n", myname);
    exit(0);
}

static int cli_gui_get_spin_value(cli_gui_t* ctx, spin_t spin) {
    return ctx->spins[spin];
}

static void cli_gui_set_spin_value(cli_gui_t* ctx, spin_t spin, int value) {
    ctx->spins[spin] = value;
}

static void cli_gui_main_loop(cli_gui_t* ctx) {
    int c;

    /* parse command line */
    while ((c = getopt(ctx->argc, ctx->argv, "hi:n:p:r:")) != - 1) {
        switch (c) {
        case 'h': printhelp(ctx->argv[0]); break;
        case 'i': ctx->spins[SPIN_INTERVAL] = atoi(optarg); break;
        case 'n': ctx->spins[SPIN_NUMBER] = atoi(optarg); break;
        case 'p': ctx->spins[SPIN_PREDELAY] = atoi(optarg); break;
        case 'r': ctx->spins[SPIN_RANDOM] = atoi(optarg); break;
        case '?':
            return;
        default:
            fprintf(stderr, "unknown command line option: %s\n", ctx->argv[optind]);
            return;
        }
    }

    common_start_button();

    while (sleeptime) {
        usleep(sleeptime * 1000);
        sleeptime = 0;
        common_alarm_callback();
    }
}

static void cli_gui_close(cli_gui_t* ctx) {
    free(ctx);
}

/* xAutoClick -- ASCII GUI
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

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "gui.h"
#include "main.h"
#include "osdep.h"

typedef struct ascii_gui_ctx {
    int spins[SPINS_COUNT];
} ascii_gui_t;

static int sleeptime;

static void ascii_gui_main_loop(ascii_gui_t* ctx);

static int ascii_gui_get_spin_value(ascii_gui_t* ctx, spin_t spin) {
    return ctx->spins[spin];
}

static void ascii_gui_set_spin_value(ascii_gui_t* ctx, spin_t spin, int value) {
    ctx->spins[spin] = value;
}

static void ascii_gui_close(ascii_gui_t* ctx) {
    set_options();
    free(ctx);
}

void init_gui(gui_t* gui, int argc, char **argv) {
    ascii_gui_t* ctx;

    ctx = calloc(1, sizeof(ascii_gui_t));
    if (!ctx) {
        fprintf(stderr, "Can't allocate memory for ASCII GUI\n");
        return;
    }

    printf("aautoclick\n\n");
    printf("p - set pre-delay, ms\n");
    printf("i - set interval, ms\n");
    printf("r - set random +/-, ms\n");
    printf("n - set number of clicks\n");
    printf("t - tap\n");
    printf("s - start\n");
    printf("q - quit\n");
    printf("\n");

    gui->ctx = ctx;
    gui->get_spin_value = (gui_get_spin_value_t)ascii_gui_get_spin_value;
    gui->set_spin_value = (gui_set_spin_value_t)ascii_gui_set_spin_value;
    gui->main_loop = (gui_main_loop_t)ascii_gui_main_loop;
    gui->close = (gui_close_t)ascii_gui_close;

    get_options(gui);
}

void set_alarm(int ms) {
    sleeptime = ms;
}

static void print_variables(ascii_gui_t* ctx) {
    printf("pre-delay:          %i ms\n", ctx->spins[SPIN_PREDELAY]);
    printf("interval:           %i ms\n", ctx->spins[SPIN_INTERVAL]);
    printf("random +/-:         %i ms\n", ctx->spins[SPIN_RANDOM]);
    printf("number of clicks:   %i\n", ctx->spins[SPIN_NUMBER]);
    printf("\n");
}

#define is_eol(x) ((x)=='\n')

static void flush_to_eol(int c) {
    if (is_eol(c))
        return;
    while(!is_eol(c)) { c = fgetc(stdin); }
}

static void read_int(int *ptr) {
    int result;
    printf("new value: ");
    result = fscanf(stdin, "%i", ptr);
    if (result != 1)
    {
        fprintf(stderr, "Input value must be integer");
        if (EOF == result)
        {
            int stdin_err = ferror(stdin);

            if (stdin_err != 0)
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
    flush_to_eol(0);
}

static void run(void) {
    common_start_button();

    while (sleeptime) {
        usleep(sleeptime * 1000);
        sleeptime = 0;
        common_alarm_callback();
    }
}

static void ascii_gui_main_loop(ascii_gui_t* ctx) {
    int c;

    print_variables(ctx);
    printf("> ");
    c = fgetc(stdin);

    while (c) {
        while(isblank(c)) { c = fgetc(stdin); }

        if (is_eol(c)) {
            printf("> ");
            c = fgetc(stdin);
            continue;
        }

        flush_to_eol(c);

        switch (c) {
        case 'p': read_int(&ctx->spins[SPIN_PREDELAY]); break;
        case 'i': read_int(&ctx->spins[SPIN_INTERVAL]); break;
        case 'r': read_int(&ctx->spins[SPIN_RANDOM]);   break;
        case 'n': read_int(&ctx->spins[SPIN_NUMBER]);   break;
        case 't': common_tap_button();             break;
        case 's': run();                           return;
        case 'q':                                  return;
        default:  printf("unknown command\n");     break;
        }

        print_variables(ctx);

        usleep(100 * 1000);
        printf("> ");
        c = fgetc(stdin);
    }

    run();
}

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
#include <string.h>
#include <errno.h>

#include "clicker.h"
#include "main.h"
#include "osdep.h"

static clicker_t *clicker;
static int spins[SPINS_COUNT], sleeptime;

void click_mouse_button(void) {
    clicker_click(clicker);
}

void set_alarm(int ms) {
    sleeptime = ms;
}

int get_spin_value(spin_t spin) {
    return spins[spin];
}

void set_spin_value(spin_t spin, int value) {
    spins[spin] = value;
}

void set_button_sensitive(button_t button, bool state) {
}

int init_gui(int argc, char **argv) {
    clicker = clicker_init();
    if (!clicker) {
        fprintf(stderr, "Unable to initialize clicker\n");
        return 0;
    }

    printf("aautoclick\n\n");
    printf("p - set pre-delay\n");
    printf("i - set interval\n");
    printf("r - set random +/-\n");
    printf("n - set number of clicks\n");
    printf("t - tap\n");
    printf("s - start\n");
    printf("q - quit\n");
    printf("\n");

    get_options();

    return 1;
}

void close_gui(void) {
    set_options();
    clicker_close(clicker);
}

static void print_variables(void) {
    printf("pre-delay:          %i\n", spins[SPIN_PREDELAY]);
    printf("interval:           %i\n", spins[SPIN_INTERVAL]);
    printf("random +/-:         %i\n", spins[SPIN_RANDOM]);
    printf("number of clicks:   %i\n", spins[SPIN_NUMBER]);
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

void main_loop(void) {
    int c;

    print_variables();
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
        case 'p': read_int(&spins[SPIN_PREDELAY]); break;
        case 'i': read_int(&spins[SPIN_INTERVAL]); break;
        case 'r': read_int(&spins[SPIN_RANDOM]);   break;
        case 'n': read_int(&spins[SPIN_NUMBER]);   break;
        case 't': common_tap_button();             break;
        case 's': run();                           return;
        case 'q':                                  return;
        default:  printf("unknown command\n");     break;
        }

        print_variables();

        usleep(100 * 1000);
        printf("> ");
        c = fgetc(stdin);
    }

    run();
}

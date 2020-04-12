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

#include "main.h"
#include "osdep.h"
#include "x11clicker.h"

static Display *display;
static int spins[SPINS_COUNT], sleeptime;

void click_mouse_button(void) {
    x11_clicker_click_mouse_button(display);
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

static void printhelp(char *myname) {
    printf("usage: %s [-h][-i value][-n value][-p value][-r value]\n", myname);
    exit(0);
}

#define option_with_argument(v,m,c) \
            (c)++; \
            if ((c) == argc) { \
                fprintf(stderr, "option %s needs an argument\n", (m)); \
                return 0; \
            } \
            (v) = atoi(argv[(c)]);

int init_gui(int argc, char **argv) {
    int c;

    display = x11_clicker_open_display();
    if (!display) {
        fprintf(stderr, "Unable to open X display\n");
        return 0;
    }

    get_options();

    /* parse command line */

    c=1;

    while (c<argc) {

        if (!strcmp(argv[c], "--help")) {
            printhelp(argv[0]);
        } else if (!strcmp(argv[c], "-help")) {
            printhelp(argv[0]);
        } else if (!strcmp(argv[c], "-h")) {
            printhelp(argv[0]);
        } else if (!strcmp(argv[c], "-i")) {
            option_with_argument(spins[SPIN_INTERVAL], "-i", c);
        } else if (!strcmp(argv[c], "-n")) {
            option_with_argument(spins[SPIN_NUMBER], "-n", c);
        } else if (!strcmp(argv[c], "-p")) {
            option_with_argument(spins[SPIN_PREDELAY], "-p", c);
        } else if (!strcmp(argv[c], "-r")) {
            option_with_argument(spins[SPIN_RANDOM], "-r", c);
        } else {
            fprintf(stderr, "unknown command line option: %s\n", argv[c]);
            return 0;
        }

        c++;
    }

    return 1;
}

void close_gui(void) {
    /* don't set options from command line */
    x11_clicker_close_display(display);
}

void main_loop(void) {

    common_start_button();

    while (sleeptime) {
        usleep(sleeptime * 1000);
        sleeptime = 0;
        common_alarm_callback();
    }

    return;
}

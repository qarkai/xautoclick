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

#include "main.h"
#include "osdep.h"

static int spins[SPINS_COUNT], sleeptime;

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

int init_gui(int argc, char **argv) {
    int c;

    get_options();

    /* parse command line */
    while ((c = getopt(argc, argv, "hi:n:p:r:")) != - 1) {
        switch (c) {
        case 'h': printhelp(argv[0]); break;
        case 'i': spins[SPIN_INTERVAL] = atoi(optarg); break;
        case 'n': spins[SPIN_NUMBER] = atoi(optarg); break;
        case 'p': spins[SPIN_PREDELAY] = atoi(optarg); break;
        case 'r': spins[SPIN_RANDOM] = atoi(optarg); break;
        case '?': return 0;
        default: fprintf(stderr, "unknown command line option: %s\n", argv[optind]); return 0;
        }
    }

    return 1;
}

void close_gui(void) {
    /* don't set options from command line */
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

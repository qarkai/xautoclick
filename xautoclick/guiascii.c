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

#include <stdio.h>
#include <X11/Xlib.h>
#include <X11/extensions/XTest.h>

#include "main.h"
#include "osdep.h"

static Display *display;
static int predelay, interval, randomfactor, numberofclicks, sleeptime;

void click_mouse_button(void) {
    XTestFakeButtonEvent(display, 1, True, CurrentTime);
    XTestFakeButtonEvent(display, 1, False, CurrentTime);
    XFlush(display);
}

void set_alarm(int ms) {
    sleeptime = ms;
}

int get_spin_value(spin_t spin) {

    switch(spin) {
    case SPIN_PREDELAY:
        return predelay;
        break;
    case SPIN_INTERVAL:
        return interval;
        break;
    case SPIN_RANDOM:
        return randomfactor;
        break;
    case SPIN_NUMBER:
        return numberofclicks;
        break;
    }

    return 0;
}

void set_spin_value(spin_t spin, int value) {

    switch(spin) {
    case SPIN_PREDELAY:     predelay       = value;     break;
    case SPIN_INTERVAL:     interval       = value;     break;
    case SPIN_RANDOM:       randomfactor   = value;     break;
    case SPIN_NUMBER:       numberofclicks = value;     break;
    }
}

void set_button_sensitive(button_t button, int state) {
}

int init_gui(int argc, char **argv) {

    if (!(display = XOpenDisplay(NULL))) {
        fprintf(stderr, "Unable to open X display\n");
        return 0;
    }

    predelay       = 2000;
    interval       = 1024;
    randomfactor   = 64;
    numberofclicks = 32;

    printf("aautoclick\n\n");
    printf("p - set pre-delay\n");
    printf("i - set interval\n");
    printf("r - set random +/-\n");
    printf("n - set number of clicks\n");
    printf("t - tap\n");
    printf("s - start\n");
    printf("q - quit\n");
    printf("\n");

    return 1;
}

void close_gui(void) {
    XCloseDisplay(display);
}

static void print_variables(void) {
    printf("pre-delay:          %i\n", predelay);
    printf("interval:           %i\n", interval);
    printf("random +/-:         %i\n", randomfactor);
    printf("number of clicks:   %i\n", numberofclicks);
    printf("\n");
}

#define is_space(x) (x==' ' || x=='\t')
#define is_eol(x) (x=='\n')

static void flush_to_eol(int c) {
    if (is_eol(c)) return;
    while(!is_eol(c)) { c = fgetc(stdin); }
}

static void read_int(int *ptr) {
    printf("new value: ");
    fscanf(stdin, "%i", ptr);
    flush_to_eol(0);
}

void main_loop(void) {
    int c;

    print_variables();
    printf("> ");
    c = fgetc(stdin);

    while (c) {

        while(is_space(c)) { c = fgetc(stdin); }
        if (is_eol(c)) {
            printf("> ");
            c = fgetc(stdin);
            continue;
        }

        flush_to_eol(c);

        switch (c) {
        case 'p':   read_int(&predelay);            break;
        case 'i':   read_int(&interval);            break;
        case 'r':   read_int(&randomfactor);        break;
        case 'n':   read_int(&numberofclicks);      break;
        case 't':   common_tap_button();            break;
        case 's':   goto run;                       break;
        case 'q':   goto quit_out;                  break;
        default:    printf("unknown command\n");    break;
        }

        print_variables();

        usleep(100 * 1000);
        printf("> ");
        c = fgetc(stdin);
    }

run:
    common_start_button();

    while (sleeptime) {
        usleep(sleeptime * 1000);
        sleeptime = 0;
        common_alarm_callback();
    }

quit_out:
    return;
}

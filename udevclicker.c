/* xAutoClick, udev mouse clicking implementation
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

#include "udevclicker.h"

#include <libevdev/libevdev-uinput.h>

#include <stdlib.h>
#include <stdio.h>

struct udev_clicker_ctx {
    struct libevdev *dev;
    struct libevdev_uinput *uidev;
};

udev_clicker_t* udev_clicker_init(void) {
    udev_clicker_t* ctx;

    ctx = malloc(sizeof(udev_clicker_t));

    ctx->dev = libevdev_new();
    libevdev_set_name(ctx->dev, "xautoclick");
    libevdev_enable_event_type(ctx->dev, EV_KEY);
    libevdev_enable_event_code(ctx->dev, EV_KEY, BTN_LEFT, NULL);
#if 0
    /* enable repeat property on key event, just like on a real keyboard */
    ioctl(uinput_fd, UI_SET_EVBIT, EV_REP);
#endif
    if (libevdev_uinput_create_from_device(ctx->dev, LIBEVDEV_UINPUT_OPEN_MANAGED, &ctx->uidev) != 0) {
        libevdev_free(ctx->dev);
        free(ctx);
        return NULL;
    }

    return ctx;
}

void udev_clicker_click_mouse_button(udev_clicker_t* ctx) {
    if (libevdev_uinput_write_event(ctx->uidev, EV_KEY, BTN_LEFT, 1) != 0) {
        fprintf(stderr, "EV_KEY/BTN_LEFT/1 failed\n");
        return;
    }

    if (libevdev_uinput_write_event(ctx->uidev, EV_SYN, SYN_REPORT, 0) != 0) {
        fprintf(stderr, "EV_SYN/SYN_REPORT/0 failed\n");
        return;
    }

    if (libevdev_uinput_write_event(ctx->uidev, EV_KEY, BTN_LEFT, 0) != 0) {
        fprintf(stderr, "EV_KEY/BTN_LEFT/0 failed\n");
        return;
    }

    if (libevdev_uinput_write_event(ctx->uidev, EV_SYN, SYN_REPORT, 0) != 0) {
        fprintf(stderr, "EV_SYN/SYN_REPORT/0 failed\n");
        return;
    }
}

void udev_clicker_destroy(udev_clicker_t* ctx) {
    if (!ctx)
        return;

    libevdev_uinput_destroy(ctx->uidev);
    libevdev_free(ctx->dev);
    free(ctx);
}

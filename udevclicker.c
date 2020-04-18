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

#include <linux/uinput.h>
#include <fcntl.h>
#include <libudev.h>
#include <unistd.h>

#include <errno.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

struct udev_clicker_ctx {
    struct uinput_user_dev user_dev;
    int uinput_fd;
};

static int suinput_open(void);
static char* suinput_get_uinput_path(void);
static int suinput_enable_mouse_click_event(int uinput_fd);
static int suinput_create(int uinput_fd, const struct uinput_user_dev *user_dev_p);
static int suinput_emit(int uinput_fd, uint16_t ev_type, uint16_t ev_code, int32_t ev_value);
static int suinput_write(int uinput_fd, const void *data, size_t data_size);
static int suinput_wait_ready(int uinput_fd);

udev_clicker_t* udev_clicker_init(void) {
    udev_clicker_t* ctx;

    ctx = malloc(sizeof(udev_clicker_t));
    memset(&ctx->user_dev, 0, sizeof(struct uinput_user_dev));
    strcpy(ctx->user_dev.name, "xautoclick");

    ctx->uinput_fd = suinput_open();
    if (ctx->uinput_fd == -1)
        return NULL;

    if (suinput_enable_mouse_click_event(ctx->uinput_fd) == -1)
        return NULL;

    if (suinput_create(ctx->uinput_fd, &ctx->user_dev) == -1)
        return NULL;

    return ctx;
}

void udev_clicker_click_mouse_button(udev_clicker_t* ctx) {
    if (suinput_emit(ctx->uinput_fd, EV_KEY, BTN_LEFT, 1) == -1) {
        fprintf(stderr, "suinput_emit key 1 failed\n");
        return;
    }

    if (suinput_emit(ctx->uinput_fd, EV_SYN, SYN_REPORT, 0) == -1) {
        fprintf(stderr, "suinput_emit syn 0 failed\n");
        return;
    }

    if (suinput_emit(ctx->uinput_fd, EV_KEY, BTN_LEFT, 0) == -1) {
        fprintf(stderr, "suinput_emit key 0 failed\n");
        return;
    }

    if (suinput_emit(ctx->uinput_fd, EV_SYN, SYN_REPORT, 0) == -1) {
        fprintf(stderr, "suinput_emit syn 0 failed\n");
        return;
    }
}

void udev_clicker_destroy(udev_clicker_t* ctx) {
    if (ioctl(ctx->uinput_fd, UI_DEV_DESTROY) == -1) {
        int ioctl_errno = errno;
        close(ctx->uinput_fd);
        errno = ioctl_errno;
        return;
    }

    close(ctx->uinput_fd);
}

static int suinput_open(void) {
    int uinput_fd;
    char *uinput_path;

    uinput_path = suinput_get_uinput_path();
    if (uinput_path == NULL)
        return -1;

    uinput_fd = open(uinput_path, O_WRONLY | O_NONBLOCK);
    free(uinput_path);
    return uinput_fd;
}

static char* suinput_get_uinput_path(void) {
    struct udev *udev;
    struct udev_device *udev_dev;
    char *uinput_path = NULL;
    int orig_errno;

    udev = udev_new();
    if (udev == NULL)
        return NULL;

    udev_dev = udev_device_new_from_subsystem_sysname(udev, "misc", "uinput");
    if (udev_dev != NULL) {
        const char *devnode = udev_device_get_devnode(udev_dev);
        if (devnode != NULL) {
            uinput_path = malloc(strlen(devnode) + 1);
            if (uinput_path != NULL)
                strcpy(uinput_path, devnode);
        }
    }

    orig_errno = errno;
    udev_device_unref(udev_dev);
    udev_unref(udev);
    errno = orig_errno;
    return uinput_path;
}

static int suinput_enable_mouse_click_event(int uinput_fd) {
    if (ioctl(uinput_fd, UI_SET_EVBIT, EV_KEY) == -1)
        return -1;

    /* enable repeat property on key event, just like on a real keyboard */
    ioctl(uinput_fd, UI_SET_EVBIT, EV_REP);

    return ioctl(uinput_fd, UI_SET_KEYBIT, BTN_LEFT);
}

static int suinput_create(int uinput_fd, const struct uinput_user_dev *user_dev_p) {
    if (suinput_write(uinput_fd, user_dev_p, sizeof(struct uinput_user_dev)) == -1)
        return -1;

    if (ioctl(uinput_fd, UI_DEV_CREATE) == -1)
        return -1;

    return 0;
}

static int suinput_emit(int uinput_fd, uint16_t ev_type, uint16_t ev_code, int32_t ev_value) {
   struct input_event event;

   memset(&event, 0, sizeof(event));
   gettimeofday(&event.time, 0);
   event.type = ev_type;
   event.code = ev_code;
   event.value = ev_value;

   return suinput_write(uinput_fd, &event, sizeof(event));
}

static int suinput_write(int uinput_fd, const void *data, size_t data_size) {
    ssize_t bytes;

    if (suinput_wait_ready(uinput_fd) == -1)
        return -1;

    bytes = write(uinput_fd, data, data_size);
    if (bytes != data_size)
        return -1;

    return 0;
}

static int suinput_wait_ready(int uinput_fd) {
    fd_set wfds;
    int res;

    FD_ZERO(&wfds);
    FD_SET(uinput_fd, &wfds);

    res = select(uinput_fd + 1, NULL, &wfds, NULL, NULL);
    if (res <= 0 && errno != EINTR)
      return -1;

    return 0;
}

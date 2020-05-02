#ifndef __UDEV_CLICKER_H__
#define __UDEV_CLICKER_H__

#include "clicker.h"

typedef struct udev_clicker_ctx udev_clicker_t;

void udev_clicker_init(clicker_t* clicker);
void udev_clicker_click_mouse_button(udev_clicker_t* ctx);
void udev_clicker_destroy(udev_clicker_t* ctx);

#endif /* __UDEV_CLICKER_H__ */

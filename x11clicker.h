#ifndef __X11_CLICKER_H__
#define __X11_CLICKER_H__

#include "clicker.h"

#include <X11/Xlib.h>

void x11_clicker_init(clicker_t* clicker);
void x11_clicker_click_mouse_button(Display* display);
void x11_clicker_close_display(Display* display);

#endif /* __X11_CLICKER_H__ */

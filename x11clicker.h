#ifndef __X11_CLICKER_H__
#define __X11_CLICKER_H__

#include <X11/Xlib.h>

Display* x11_clicker_open_display(void);
void x11_clicker_click_mouse_button(Display* display);
void x11_clicker_close_display(Display* display);

#endif /* __X11_CLICKER_H__ */

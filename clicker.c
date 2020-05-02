#include "clicker.h"

#include "udevclicker.h"
#include "x11clicker.h"

#include <stdlib.h>

typedef enum clicker_type_e {
    CLICKER_UDEV = 0,
    CLICKER_X11
} clicker_type_t;

static clicker_t* clicker_create(clicker_type_t type) {
    clicker_t* clicker;

    clicker = calloc(1, sizeof(clicker_t));
    if (!clicker)
        return NULL;

    switch (type) {
    case CLICKER_UDEV: udev_clicker_init(clicker); break;
    case CLICKER_X11: x11_clicker_init(clicker); break;
    default: break;
    }

    if (!clicker->ctx) {
        free(clicker);
        return NULL;
    }

    return clicker;
}

clicker_t* clicker_init(void) {
    clicker_t* clicker;

    clicker = clicker_create(CLICKER_UDEV);
    if (clicker)
        return clicker;

    return clicker_create(CLICKER_X11);
}

void clicker_close(clicker_t* clicker) {
    if (!clicker)
        return;

    if (clicker->close)
        clicker->close(clicker->ctx);

    free(clicker);
}

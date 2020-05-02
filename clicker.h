#ifndef __CLICKER_H__
#define __CLICKER_H__

typedef void (*clicker_click_t)(void* ctx);
typedef void (*clicker_close_t)(void* ctx);

typedef enum clicker_type_e {
    CLICKER_UDEV = 0,
    CLICKER_X11
} clicker_type_t;

typedef struct clicker_ctx {
    void* ctx;

    clicker_click_t click;
    clicker_close_t close;
} clicker_t;

clicker_t* clicker_create(clicker_type_t type);

static inline void clicker_click(clicker_t* clicker) {
    clicker->click(clicker->ctx);
}

void clicker_close(clicker_t* clicker);

#endif /* __CLICKER_H__ */

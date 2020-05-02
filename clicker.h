#ifndef __CLICKER_H__
#define __CLICKER_H__

typedef void (*clicker_click_t)(void* ctx);
typedef void (*clicker_close_t)(void* ctx);

typedef struct clicker_ctx {
    void* ctx;

    clicker_click_t click;
    clicker_close_t close;
} clicker_t;

clicker_t* clicker_init(void);

static inline void clicker_click(clicker_t* clicker) {
    clicker->click(clicker->ctx);
}

void clicker_close(clicker_t* clicker);

#endif /* __CLICKER_H__ */

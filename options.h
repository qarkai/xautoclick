#ifndef __OPTIONS_H__
#define __OPTIONS_H__

typedef struct options {
    int predelay;
    int interval;
    int random_factor;
    int clicks_number;
} options_t;

char* get_config_file(void);
void load_options(const char* filename, options_t *opts);
void save_options(const char* filename, const options_t *opts);

#endif /* __OPTIONS_H__ */

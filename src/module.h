#ifndef MODULE_H
#define MODULE_H

typedef void (*module_init_t)(void);
typedef void (*module_entry_t)(void);

typedef struct {
    const char *name;
    module_init_t init;
    module_entry_t entry;
} module_t;

#define MODULE(SYMBOL) static module_t SYMBOL __attribute__((section("modules"), used))

#endif

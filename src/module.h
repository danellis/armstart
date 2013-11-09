typedef void (*moduleinit)(void);

typedef struct moduledesc {
    const char *name;
    moduleinit init;
} moduledesc;

#define MODULE(SYMBOL) static moduledesc SYMBOL __attribute__((section("modules")))

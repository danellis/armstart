#ifndef MODULE_H
#define MODULE_H

typedef void (*module_init_t)(void);
typedef void (*module_entry_t)(void);
typedef void (*module_svc_handler_t)(uint32_t, uint32_t *);

typedef struct {
    const char *name;
    module_init_t init;
    module_entry_t entry;
    uint32_t svc_mask;
    uint32_t svc_number;
    module_svc_handler_t svc_handler;
} module_t;

extern module_t __modules_start[];
extern module_t __modules_end[];

#define MODULE(SYMBOL) static module_t SYMBOL __attribute__((section("modules"), used))

#define SVC(N, R, F, ...) __attribute__((naked, noinline)) R F(__VA_ARGS__) { __asm__("svc " #N " >> 16; .2byte " #N " & 0xffff"); }

#define foreach_module(S) for (module_t *S = __modules_start; S < __modules_end; ++S)

#endif

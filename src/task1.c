#include "module.h"
#include "debug.h"

__attribute__((naked, noreturn)) static void task1_entry(void) {
    for (;;) {
        debug_putc('1');
        for (int i = 0; i < 0x10000; ++i);
    }
}

MODULE(task1_module) = {
    "task1",
    0,
    task1_entry
};

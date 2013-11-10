#include "module.h"
#include "debug.h"

__attribute__((naked, noreturn)) static void task2_entry(void) {
    for (;;) {
        debug_putc('2');
        for (int i = 0; i < 0x10000; ++i);
    }
}

MODULE(task2_module) = {
    "task2",
    0,
    task2_entry
};

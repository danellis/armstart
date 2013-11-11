#include "module.h"
#include "debug.h"
#include "uart.h"

__attribute__((naked, noreturn)) static void task1_entry(void) {
    debug_puts("Start of task 1\n");
    for (;;) {
        Uart_Write("1");
        for (int i = 0; i < 0x1000; ++i);
    }
}

MODULE(task1_module) = {
    "task1",
    0,
    task1_entry
};

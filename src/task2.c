#include "module.h"
#include "debug.h"
#include "uart.h"

__attribute__((naked, noreturn)) static void task2_entry(void) {
    debug_puts("Start of task 2\n");
    for (;;) {
        Uart_Write("2");
        for (int i = 0; i < 0x1000; ++i);
    }
}

MODULE(task2_module) = {
    "task2",
    0,
    task2_entry
};

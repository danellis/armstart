#include "module.h"

static void uart_init(void) {
}

MODULE(uart_module) = {
    "uart",
    uart_init
};

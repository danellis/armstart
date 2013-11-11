#include <stdio.h>
#include "module.h"
#include "debug.h"

static void uart_putc(char c) {
    LPC_UART_TypeDef *uart = LPC_UART3;
    while ((uart->LSR & (1 << 5)) == 0);
    uart->THR = c;
}

static void uart_puts(const char *s) {
    char c;
    while ((c = *s++)) {
        uart_putc(c);
    }
}

static void uart_init(void) {
    LPC_SC->PCONP |= 1 << 25;
    LPC_PINCON->PINSEL1 |= 0b1111 << 18;
    LPC_UART_TypeDef *uart = LPC_UART3;
    uart->FCR = 0b111; // Reset FIFOs
    uart->FCR = 0; // Disable FIFOs
    uart->LCR = 0b10000011;
    uart->DLL = 12;
    uart->DLM = 0;
    uart->FDR = 0xe5;
    uart->LCR = 0b00000011;
}

static void uart_svc_handler(uint32_t *sp) {
    uart_puts((const char *) sp[0]);
}

MODULE(uart_module) = {
    "uart",
    uart_init,
    0, // No task
    1, // SVC number
    uart_svc_handler
};

SVC(1, void, Uart_Write, const char *str);

// void Uart_Write(const char *str) {
//     uart_puts(str);
// }

void debug_init(void) {
    LPC_PINCON->PINSEL0 = 0b00010100;

    LPC_UART_TypeDef *uart = LPC_UART0;
    uart->FCR = 0b111; // Reset FIFOs
    uart->FCR = 0; // Disable FIFOs
    uart->LCR = 0b10000011;
    uart->DLL = 12;
    uart->DLM = 0;
    uart->FDR = 0xe5;
    uart->LCR = 0b00000011;
}

void debug_putc(char c) {
    LPC_UART_TypeDef *uart = LPC_UART0;
    while ((uart->LSR & (1 << 5)) == 0);
    uart->THR = c;
}

void debug_puts(const char *s) {
    char c;
    while ((c = *s++)) {
        debug_putc(c);
    }
}

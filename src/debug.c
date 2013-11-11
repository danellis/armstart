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
    while ((uart->LSR & (1 << 5)) == 0);
}

void debug_puts(const char *s) {
    char c;
    while ((c = *s++)) {
        debug_putc(c);
    }
}

static const char *hex_chars = "0123456789abcdef";

void debug_putptr(void *ptr) {
    uint32_t i = (uint32_t) ptr;
    for (int n = 28; n >=0; n -= 4) {
        debug_putc(hex_chars[(i >> n) & 0xf]);
    }
}

void debug_put8(uint8_t value) {
    debug_putc(hex_chars[value >> 8]);
    debug_putc(hex_chars[value & 0xf]);
}

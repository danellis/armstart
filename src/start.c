extern int __bss_start;
extern int __bss_end;
extern int __data_init_start;
extern int __data_start;
extern int __data_end;
extern int __stack_end;
extern int main(void);

static void __startup(void) {
    // Zero out .bss section
    for (int *ptr = &__bss_start; ptr < &__bss_end; ++ptr) {
        *ptr = 0;
    }

    // Initialize .data section
    for (int *src = &__data_init_start, *dest = &__data_start; dest < &__data_end;) {
        *dest++ = *src++;
    }

    // Jump to main
    main();

    for (;;);
}

static void *__vectors[16] __attribute__((section(".vectors"))) = {
    &__stack_end,
    __startup,
    (void *) 0x12345678
};

extern int __bss_start;
extern int __bss_end;
extern int __data_init_start;
extern int __data_start;
extern int __data_end;
extern int __stack_end;
extern int main(void);

static void __startup(void) {
    LPC_GPIO0->DIR |= 1 << 8; // GPIO0_8 is an output
    LPC_GPIO0->DATA &= ~(1 << 8); // LED off

    // Zero out .bss section
    for (int *ptr = &__bss_start; ptr < &__bss_end; ++ptr) {
        *ptr = 0;
    }

    // Initialize .data section
    for (int *src = &__data_init_start, *dest = &__data_start; dest < &__data_end;) {
        *dest++ = *src++;
    }

    // Initialize system clocks
    SystemInit();

    // Jump to main
    main();

    for (;;);
}

void Dummy_Handler(void) {
    for (;;);
}

#define WEAK_ALIAS(x) __attribute__ ((weak, alias(#x)))

void NMI_Handler(void) WEAK_ALIAS(Dummy_Handler);
void HardFault_Handler(void) WEAK_ALIAS(Dummy_Handler);
void MemManage_Handler(void) WEAK_ALIAS(Dummy_Handler);
void BusFault_Handler(void) WEAK_ALIAS(Dummy_Handler);
void UsageFault_Handler(void) WEAK_ALIAS(Dummy_Handler);
void SVC_Handler(void) WEAK_ALIAS(Dummy_Handler);
void DebugMon_Handler(void) WEAK_ALIAS(Dummy_Handler);
void PendSV_Handler(void) WEAK_ALIAS(Dummy_Handler);
void SysTick_Handler(void) WEAK_ALIAS(Dummy_Handler);

static void *__vectors[16] __attribute__((section(".vectors"), unused)) = {
    &__stack_end,
    __startup,
    NMI_Handler,
    HardFault_Handler,
    MemManage_Handler,
    BusFault_Handler,
    UsageFault_Handler,
    0,
    0,
    0,
    0,
    SVC_Handler,
    DebugMon_Handler,
    0,
    PendSV_Handler,
    SysTick_Handler,
};

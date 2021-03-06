#include "module.h"
#include "debug.h"
#include "task.h"

extern int __bss_start;
extern int __bss_end;
extern int __data_init_start;
extern int __data_start;
extern int __data_end;
extern int __main_stack_end;

extern int main(void);

static void __startup(void) {
    // Exception LED
    LPC_GPIO2->FIODIR |= 1 << 13;
    LPC_GPIO2->FIOCLR |= 1 << 13;

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

    debug_init();
    debug_puts(" _    ________________________\n| |  / / ____/ ____/_  __/ __ \\\n| | / / __/ / /     / / / /_/ /\n");
    debug_puts("| |/ / /___/ /___  / / / _, _/\n|___/_____/\\____/ /_/ /_/ |_|\n");
    debug_puts("Firmware booting\n");

    // Call constructors
    debug_puts("Initializing modules: ");
    foreach_module(module) {
        if (module->init) {
            debug_puts(module->name);
            debug_putc(' ');
            module->init();
        }
    }
    debug_putc('\n');

    debug_puts("Starting module tasks: ");
    foreach_module(module) {
        if (module->entry) {
            debug_puts(module->name);
            debug_putc(' ');
            task_create(module->entry);
        }
    }
    debug_putc('\n');

    // Become task0 (idle task)
    become_task0();

    for (;;);
}

void Dummy_Handler(void) {
    // Light exception LED
    LPC_GPIO2->FIOSET |= 1 << 13;
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
    &__main_stack_end,
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

#include <string.h>
#include "task.h"
#include "debug.h"

extern stack_t __ps_stack_end[MAX_TASKS];

task_t tasks[MAX_TASKS];

static int current_task;

void *save_context(void) {
    register void *sp asm("r0");
    __asm__ ("mrs %0, psp\n\tstmfd %0!, {r4-r11}" :: "r" (sp));
    return sp;
}

__attribute__((naked)) void SysTick_Handler(void) {
    __asm__("push {lr}");
    void *sp = save_context();
    tasks[current_task].sp = sp;
    if (++current_task == MAX_TASKS) current_task = 0;
    sp = tasks[current_task].sp;

    __asm__("mov r0, r4\n\tldmfd r0!, {r4-r11}");
    __asm__("msr psp, r0");
    __asm__("pop {pc}");
}

__attribute__((naked, noreturn)) static void task0(void) {
    for (;;) {
        // if (LPC_GPIO0->FIOPIN & (1 << 22)) {
        //     LPC_GPIO0->FIOCLR |= 1 << 22;
        // } else {
        //     LPC_GPIO0->FIOSET |= 1 << 22;
        // }
        debug_putc('0');
        for (int i = 0; i < 0x100000; ++i);
    }
}

__attribute__((naked, noreturn)) static void task1(void) {
    for (;;) {
        debug_putc('1');
        for (int i = 0; i < 0x40000; ++i);
    }
}

__attribute__((naked, noreturn)) static void task2(void) {
    for (;;) {
        debug_putc('2');
        for (int i = 0; i < 0x10000; ++i);
    }
}

__attribute__((naked, noreturn)) static void task3(void) {
    for (;;) {
        debug_putc('3');
        for (int i = 0; i < 0x10000; ++i);
    }
}

static void task_init(task_t *task, task_entry_t entry, void *stack) {
    task_context_t *ctx = stack - sizeof(task_context_t);
    ctx->hw.r0 = 0;
    ctx->hw.r1 = 0;
    ctx->hw.r2 = 0;
    ctx->hw.r3 = 0;
    ctx->hw.r12 = 0;
    ctx->hw.pc = entry;
    ctx->hw.lr = 0;
    ctx->hw.psr = 0x21000000;
    task->sp = stack - sizeof(hw_context_t) - sizeof(sw_context_t);
}

void init_tasks(void) {
    memset(tasks, 0, MAX_TASKS * sizeof(task_t));

    // task_init(&tasks[0], task0, &__ps_stack_end[0]);
    debug_puts("  task 0 (idle task)\n");
    tasks[0].sp = &__ps_stack_end[0];

    task_entry_t task_entries[MAX_TASKS] = {task0, task1, task2, task3};

    for (int t = 1; t < MAX_TASKS; ++t) {
        debug_puts("  task ");
        debug_putc('0' + t);
        debug_putc('\n');
        task_init(&tasks[t], task_entries[t], &__ps_stack_end[-t]);
    }
    current_task = 0;

    debug_puts("Starting SysTick @ 100Hz\n");
    SysTick_Config(SystemCoreClock / 100);

    debug_puts("Becoming task 0 (idle task)\n");
    __set_PSP((uint32_t) &__ps_stack_end[0]);
    __set_CONTROL(0b11);
    task0();
}

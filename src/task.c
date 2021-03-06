#include <string.h>
#include "task.h"
#include "debug.h"

extern stack_t __ps_stack_end[];

task_t tasks[MAX_TASKS];
static int current_task = 0;
static int num_tasks = 1;

void schedule(void) {
    if (++current_task == num_tasks) current_task = 0;
}

__attribute__((naked)) void SysTick_Handler(void) {
    register void *sp __asm__("r0");

    __asm__("push {lr}");
    __asm__ ("mrs %0, psp\n\tstmfd %0!, {r4-r11}" :: "r" (sp));

    tasks[current_task].sp = sp;
    schedule();
    sp = tasks[current_task].sp;

    __asm__("ldmfd %0!, {r4-r11}; msr psp, %0; pop {pc}" : "=r" (sp));
}

__attribute__((naked, noreturn)) static void task0(void) {
    debug_puts("Start of task 0\n");
    for (;;) {
        // if (LPC_GPIO0->FIOPIN & (1 << 22)) {
        //     LPC_GPIO0->FIOCLR |= 1 << 22;
        // } else {
        //     LPC_GPIO0->FIOSET |= 1 << 22;
        // }
        // debug_putc('0');
        // for (int i = 0; i < 0x10000; ++i);
    }
}

static void task_init_stack(task_t *task, task_entry_t entry, void *stack) {
    task_context_t *ctx = stack - sizeof(task_context_t);
    ctx->hw.r0 = 0;
    ctx->hw.r1 = 0;
    ctx->hw.r2 = 0;
    ctx->hw.r3 = 0;
    ctx->hw.r12 = 0;
    ctx->hw.pc = entry;
    ctx->hw.lr = 0;
    ctx->hw.psr = 0x21000000;
    task->sp = ctx;
}

void task_create(task_entry_t entry) {
    memset(&tasks[num_tasks], 0, sizeof(task_t));
    task_init_stack(&tasks[num_tasks], entry, &__ps_stack_end[-num_tasks]);
    ++num_tasks;
}

void become_task0(void) {
    debug_puts("Creating task 0 (idle task)\n");
    tasks[0].sp = &__ps_stack_end[0];

    debug_puts("Starting SysTick @ 100Hz\n");
    SysTick_Config(SystemCoreClock / 100);

    debug_puts("Becoming task 0\n");
    __set_PSP((uint32_t) &__ps_stack_end[0]);
    __ISB();
    __set_CONTROL(0b11);
    task0();
}

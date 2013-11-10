#include <string.h>
#include "task.h"
#include "debug.h"

extern stack_t __stack_end[MAX_TASKS];

task_t tasks[MAX_TASKS];

static int current_task;

__attribute__((naked)) static inline void *save_context(void) {
    register void *sp asm("r0");
    LPC_GPIO0->FIOSET |= 1 << 22;
    __asm__ ("mrs %0, psp; stmfd %0!, {r4-r11}" :: "r" (sp));
    return sp;
}

__attribute__((naked)) static inline void load_context(void *sp) {
    __asm__ ("ldmfd %0!, {r4-r11}; msr psp, %0" : "=r" (sp));
}

__attribute__((naked)) static void switch_task() {
    void *sp = save_context();
    tasks[current_task].sp = sp;
    if (++current_task == MAX_TASKS) current_task = 0;
    sp = tasks[current_task].sp;
    load_context(sp);
}

__attribute__((naked)) void SysTick_Handler(void) {
    switch_task();
}

static void end_of_task(void) {
}

static void task1(void) {
    for (;;) {
        // if (LPC_GPIO0->FIOPIN & (1 << 22)) {
        //     LPC_GPIO0->FIOCLR |= 1 << 22;
        // } else {
        //     LPC_GPIO0->FIOSET |= 1 << 22;
        // }
        // for (int i = 0; i < 0xf0000000; ++i);
    }
}

static void task2(void) {
    for (;;);
}

static void task_init(task_t *task, task_entry_t entry, void *stack) {
    hw_context_t *top = stack;
    top->r0 = 0;
    top->r1 = 0;
    top->r2 = 0;
    top->r3 = 0;
    top->r12 = 0;
    top->pc = entry;
    top->lr = end_of_task;
    top->psr = 0x21000000;
    task->sp = stack - sizeof(hw_context_t) - sizeof(sw_context_t);
}

void init_tasks(void) {
    memset(tasks, 0, MAX_TASKS * sizeof(task_t));

    debug_puts("  task 1\n");
    task_init(&tasks[0], task1, &__stack_end[0]);
    debug_puts("  task 2\n");
    task_init(&tasks[1], task2, &__stack_end[-1]);

    // load_context(tasks[0].sp);

    debug_puts("Starting SysTick @ 10MHz\n");
    SysTick_Config(SystemCoreClock / 10);
}

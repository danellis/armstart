#ifndef TASK_H
#define TASK_H

#define STACK_SIZE 1024
#define MAX_TASKS 8

extern void init_tasks(void);

typedef void (*task_entry_t)(void);
void task_create(task_entry_t entry);
void become_task0(void);

typedef struct {
    uint32_t r0;
    uint32_t r1;
    uint32_t r2;
    uint32_t r3;
    uint32_t r12;
    void *lr;
    void *pc;
    uint32_t psr;
} hw_context_t;

typedef struct {
    uint32_t r4;
    uint32_t r5;
    uint32_t r6;
    uint32_t r7;
    uint32_t r8;
    uint32_t r9;
    uint32_t r10;
    uint32_t r11;
} sw_context_t;

typedef struct {
    sw_context_t sw;
    hw_context_t hw;
} task_context_t;

typedef struct {
    uint8_t bottom[STACK_SIZE - sizeof(hw_context_t)];
    hw_context_t top;
} stack_t;

typedef struct {
    void *sp;
    struct {
    } flags;
} task_t;

#endif

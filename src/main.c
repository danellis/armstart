#include <stdbool.h>
#include "task.h"
#include "debug.h"

int main(void) {
    LPC_GPIO0->FIODIR |= 1 << 22;
    LPC_GPIO0->FIOCLR |= 1 << 22;

    debug_puts("Initializing tasks\n");
    init_tasks();
}

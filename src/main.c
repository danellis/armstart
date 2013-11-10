#include <stdbool.h>
#include "task.h"

int main(void) {
    LPC_GPIO0->FIODIR |= 1 << 22;
    LPC_GPIO0->FIOCLR |= 1 << 22;

    init_tasks();
}

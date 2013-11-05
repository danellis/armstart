#include <stdbool.h>

int main(void) {
    LPC_GPIO1->DIR |= 1 << 8; // GPIO1_8 is an output
    LPC_GPIO1->DATA |= 1 << 8; // LED on

    SysTick_Config(SystemCoreClock / 10);
}

void SysTick_Handler(void) {
    LPC_GPIO1->DATA ^= 1 << 8;
}

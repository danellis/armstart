#include <stdbool.h>

int main(void) {
    LPC_GPIO0->FIODIR |= 1 << 22;
    LPC_GPIO0->FIOSET |= 1 << 22;

    SysTick_Config(SystemCoreClock / 10);
}

void SysTick_Handler(void) {
    if (LPC_GPIO0->FIOPIN & (1 << 22)) {
        LPC_GPIO0->FIOCLR |= 1 << 22;
    } else {
        LPC_GPIO0->FIOSET |= 1 << 22;
    }
}

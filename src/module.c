#include "debug.h"
#include "module.h"

static void handle_svc(uint32_t *sp) {
    uint32_t pc = sp[6];
    uint8_t svc_number = *(uint8_t *) (pc - 2);

    int dispatched = 0;
    foreach_module(module) {
        if (module->svc_number == svc_number) {
            dispatched = 1;
            module->svc_handler(sp);
            break;
        }
    }
    if (!dispatched) {
        debug_puts("No SVC handler found\n");
    }
}

__attribute__((naked)) void SVC_Handler(void) {
    __asm__(
        "tst lr, #4; ite eq; mrseq r0, msp; mrsne r0, psp; b %[handle_svc]"
        :
        : [handle_svc] "i" (handle_svc)
        : "r0"
    );
}

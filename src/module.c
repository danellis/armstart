#include "debug.h"
#include "module.h"

static void handle_svc(uint32_t *sp) {
    sp[5] += 2; // Skip over SVC LSBs
    uint8_t *pc = (uint8_t *) (sp[6] - 2);
    uint32_t svc_number = (pc[0] << 16) | (pc[3] << 8) | (pc[2]) ;

    int dispatched = 0;
    foreach_module(module) {
        if (module->svc_number == (svc_number & module->svc_mask)) {
            dispatched = 1;
            module->svc_handler(svc_number & ~module->svc_mask, sp);
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

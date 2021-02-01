/*
 * irq.s
 *
 *  Created on: Jan 24, 2021
 *      Author: ogruber
 */

_vector_start:
 LDR PC, reset_handler_addr
 LDR PC, undef_handler_addr
 LDR PC, swi_handler_addr
 LDR PC, prefetch_abort_handler_addr
 LDR PC, data_abort_handler_addr
 B .
 LDR PC, irq_handler_addr
 LDR PC, fiq_handler_addr

reset_handler_addr: .word _entry
undef_handler_addr: .word _undef_handler
swi_handler_addr: .word _swi_handler
prefetch_abort_handler_addr: .word _prefetch_abort_handler
data_abort_handler_addr: .word _data_abort_handler
irq_handler_addr: .word _irq_handler
fiq_handler_addr: .word _fiq_handler

_vector_end:


.global _wfi
_wfi:
	MCR p15,0,r0,c7,c0,4
	mov pc,lr

/*
 * Initial setup for handling interrupts on the processor
 * It is about setting up the stack for the interrupt mode.
 */
.global _irqs_setup
_irqs_setup:
    /* get Program Status Register */
    mrs r0, cpsr
    /* go in IRQ mode */
    bic r1, r0, #0x1F
    orr r1, r1, #0x12
    msr cpsr, r1
    /* set IRQ stack */
    ldr sp, =irq_stack_top
    /* go back in Supervisor mode */
    msr cpsr, r0
    mov pc,lr

/*
 * Enable all interrupts at the processor.
 */
.global _irqs_enable
_irqs_enable:
    /* get Program Status Register */
    mrs r0, cpsr
    /* Enable IRQs on the processor by clearing the mask bit
     * The BIC (BIt Clear) instruction performs an AND operation
     * with the complements of the given constants #0x80.
     */
    bic r0, r0, #0x80
    /* go back in Supervisor mode */
    msr cpsr, r0
    mov pc,lr

/*
 * Disable all interrupts at the processor and
 * at the processor only, the VIC is still managing
 * interrupts. When the interrupts will be turned back
 * on, the VIC will interrupt the processor if they are
 * pending interrupts.
 */
.global _irqs_disable
_irqs_disable:
    /* get Program Status Register */
    mrs r0, cpsr
    /* Disable IRQs on the processor */
    orr r0, r0, #0x80
    /* go back in Supervisor mode */
    msr cpsr, r0
    mov pc,lr

/*
 * Copies the exception vector to 0x0000-0000
 */
.global _copy_vector
_copy_vector:
	ldr     r3,=_vector_start
	mov		r4,#0
	cmp     r3,r4
	beq     .do_not_copy
	ldr	r9, =_vector_end
1:
	ldmia	r3!, {r5-r8} // load multiple from r3
	stmia	r4!, {r5-r8} // store multiple at r4

	cmp	r3, r9 // are we done yet?
	blo	1b
.do_not_copy:
	mov pc,lr

/*
 * The processor jumps here, in interrupt mode, when it decides
 * to handle an interrupt request from the VIC.

 * Nota Bene:
 *   The '^' symbol at the end of the last LDMFD instruction
 *   means that the CPSR will be restored from the save SPSR.
 *   The '^' restores the saved SPSR only if the PC is loaded
 *   by the LDMFD instruction.
 *   CPSR: Current Program Status Register
 *   SPSR: Saved Program Status Register
 */
_irq_handler:
	sub lr,lr,#4
	stmfd sp!, {r0-r12,lr}
	bl irq_handler
	ldmfd sp!, {r0-r12,pc}^

_fiq_handler:
	b _panic // unexpected fast interrupt

_undef_handler:
	b _panic // unexpected trap for an undefined instruction

_swi_handler:
	b _panic  // unexpected software interrupt

_prefetch_abort_handler:
	b _panic  // unexpected prefetch-abort trap

_data_abort_handler:
	b _panic  // unexpected abort trap

.global _panic
_panic:
	b _panic


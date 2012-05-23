
.code32
.extern isr_handler

/* This is our common ISR stub. It saves the processor state, sets
   up for kernel mode segments, calls the C-level fault handler,
   and finally restores the stack frame. */

.globl isr_common_stub
isr_common_stub:

   pusha                    /* Pushes edi,esi,ebp,esp,ebx,edx,ecx,eax */

   xorl %eax, %eax          /* Clear eax */

   mov %ds, %ax             /* Lower 16-bits of eax = ds. */
   push %eax                /* save the data segment descriptor */

   mov %es, %ax             /* Lower 16-bits of eax = es. */
   push %eax                /* save the extended segment descriptor */

   mov %fs, %ax             /* Lower 16-bits of eax = fs. */
   push %eax                /* save the f-segment descriptor */

   mov %gs, %ax             /* Lower 16-bits of eax = gs. */
   push %eax                /* save the g-segment descriptor */

   mov $0x10, %ax           /* load the kernel data segment descriptor */
   mov %ax, %ds
   mov %ax, %es
   mov %ax, %fs
   mov %ax, %gs

   calll isr_handler

   pop %eax       			/* reload the original g-segment descriptor */
   mov %ax, %gs

   pop %eax       			/* reload the original f-segment descriptor */
   mov %ax, %fs

   pop %eax       			/* reload the original extended segment descriptor */
   mov %ax, %es

   pop %eax       			/* reload the original data segment descriptor */
   mov %ax, %ds

   popa                     /* Pops edi,esi,ebp... */

   add $0x8, %esp           /* Cleans up the pushed error code and pushed ISR number */
   sti
   iret                     /* pops 5 things at once: CS, EIP, EFLAGS, SS, and ESP */

/* Single Step; No Error Code; Use Debug Registers */
.globl isr1
isr1:
	cli
	pushl $0x0			/* Push a dummy Error Code */
	pushl $0x1			/* Push the Interrupt Number */
	jmp isr_common_stub

/* BreakPoint; No Error Code */
.globl isr3
isr3:
	cli
	pushl $0x0			/* Push a dummy Error Code */
	pushl $0x3			/* Push the Interrupt Number */
	jmp isr_common_stub


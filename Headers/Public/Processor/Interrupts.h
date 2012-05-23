#ifndef __PROCESSOR_INTERRUPTS_H__
#define __PROCESSOR_INTERRUPTS_H__

#include <stdint.h>
#include <stdlib.h>

#define IDT_ENTERIES 256

typedef struct guest_reg_state
{
   uint32_t ds, es, fs, gs;                             /* Segment selectors */
   uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;     /* Pushed by pusha. */
   uint32_t int_no, err_code;                           /* Interrupt number and error code (if applicable) */
   uint32_t eip, cs, eflags, useresp, ss;               /* Pushed by the processor automatically. */
} guest_reg_state_t;

/* A struct describing an interrupt gate. */
struct idt_entry_struct
{
   uint16_t base_lo;             /* The lower 16 bits of the address to jump to when this interrupt fires. */
   uint16_t sel;                 /* Kernel segment selector. */
   uint8_t  always0;             /* This must always be zero. */
   uint8_t  flags;               /* More flags. See documentation. */
   uint16_t base_hi;             /* The upper 16 bits of the address to jump to. */
} __attribute__((packed));
typedef struct idt_entry_struct idt_entry_t;

/* A struct describing a pointer to an array of interrupt handlers. */
/* This is in a format suitable for giving to 'lidt'. */
struct idt_ptr_struct
{
   uint16_t limit;
   uint32_t base;                /* The address of the first element in our idt_entry_t array. */
} __attribute__((packed));
typedef struct idt_ptr_struct idt_ptr_t;

extern void isr1 (void);
extern void isr3 (void);

void cpu_init_pmode_idt();

#endif // __PROCESSOR_INTERRUPTS_H__


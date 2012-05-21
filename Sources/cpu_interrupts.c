#include <Processor/Interrupts.h>
#include <DnaTools/C.h>
#include <string.h>
#include <stdio.h>

idt_entry_t idt_entries[IDT_ENTERIES];
idt_ptr_t   idt_ptr;

static void idt_set_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags)
{
   idt_entries[num].base_lo = base & 0xFFFF;
   idt_entries[num].base_hi = (base >> 16) & 0xFFFF;

   idt_entries[num].sel     = sel;
   idt_entries[num].always0 = 0;
   /* We must uncomment the OR below when we get to using user-mode.
      It sets the interrupt gate's privilege level to 3. */
   idt_entries[num].flags   = flags /* | 0x60 */;
} 

void cpu_init_pmode_idt(void)
{
   platform_debug_puts("Init Protected Mode IDT\n");
   
   idt_ptr.limit = sizeof(idt_entry_t) * IDT_ENTERIES - 1;
   idt_ptr.base  = (uint32_t)&idt_entries;

   memset(&idt_entries, 0, sizeof(idt_entry_t) * IDT_ENTERIES);

   /* We Set ISR Gates for the following only */
   idt_set_gate(0x1, (uint32_t)isr1 , 0x10, 0x8E);
   idt_set_gate(0x3, (uint32_t)isr3 , 0x10, 0x8E);

   platform_debug_puts("Loading IDT Pointer\n");	

   __asm__ volatile ("lidtl %0" : : "m" (idt_ptr));
}

void isr_handler(registers_t regs)
{
    if (regs.int_no == 0x1)
    {
        __asm__ volatile ("movl %0,%%eax" : : "m" (regs.eip));
        __asm__ volatile ("outl %eax,$0xE1");
    }
    else if(regs.int_no == 0x3)
    {
        /* decrement eip; so we re-execute the original instruction */
        /* regs.eip -= 1; */
        /* pass control to user-space gdb server; for further processsing */
        __asm__ volatile ("movl %0,%%eax" : : "m" (regs.eip));
        __asm__ volatile ("outl %eax,$0xE3");
    }

    return;
}


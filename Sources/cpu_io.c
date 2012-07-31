
#include <Processor/Processor.h>

//inline uint64_t get_cycles (void)
uint64_t get_cycles (void)
{
    uint32_t        lo, hi;
    __asm__ __volatile__ (
        // serialize
        "xorl %%eax,%%eax\ncpuid"
        ::: "%rax", "%rbx", "%rcx", "%rdx");

    /* We cannot use "=A", since this would use %rax on x86_64 and return only the lower 32bits of the TSC */
    __asm__ __volatile__ ("rdtsc" : "=a" (lo), "=d" (hi));
 
    return ((uint64_t) hi) << 32 | lo;
}


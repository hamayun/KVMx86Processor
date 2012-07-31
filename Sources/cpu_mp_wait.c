#include <stdint.h>
#include <Processor/Processor.h>
//#include <SoclibPlatformDriver/Driver.h>

void cpu_mp_wait (void)
{    
    /*
    * TODO: add WBFLUSH
    */    

    while ((volatile int32_t) cpu_mp_synchro);
}


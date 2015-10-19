#include "gos_interface_driver.h"
#include "stdio.h"
#include "stdlib.h"
#include <string.h>
#include "register_map.h"

#define PCFG_PROG_B (1 << 30)
#define CHECK_BIT(var,pos) ((var) & (1<<(pos)))

int main(int argc, char* argv[])
{
    volatile void *lock_reg = getvaddr(0xF8007034); 
    *((volatile unsigned *)(lock_reg)) = 0x757BDF0D;

    volatile void *devcfg_ctrl= getvaddr(0xF8007000);
    *((volatile unsigned *)devcfg_ctrl) = *((volatile unsigned *)devcfg_ctrl) | PCFG_PROG_B;
    *((volatile unsigned *)devcfg_ctrl) = *((volatile unsigned *)devcfg_ctrl) & ~PCFG_PROG_B;    

    return 0; 
}


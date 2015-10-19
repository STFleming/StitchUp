#include "gos_interface_driver.h"
#include "stdio.h"
#include "stdlib.h"
#include <string.h>
#include "register_map.h"

#define CHECK_BIT(var,pos) ((var) & (1<<(pos)))

int main(int argc, char* argv[])
{
    volatile void *devcfg_status= getvaddr(0xF8007014);
    volatile void *devcfg_intsts= getvaddr(0xF800700C);
    //if(CHECK_BIT(*((volatile unsigned *)(devcfg_status)+0), 4))
    //printf("%d\n", CHECK_BIT(*((volatile unsigned *)(devcfg_status)+0), 4));

    //while(!CHECK_BIT(*((volatile unsigned *)(devcfg_intsts)), 2)) { } //Wait till the PL is ready
    //printf("SEU=%d\n", CHECK_BIT(*((volatile unsigned *)(devcfg_intsts)), 5));

    if(CHECK_BIT(*((volatile unsigned *)(devcfg_status)+0), 4)) //Check the status
    {
	return 1;
    }
    else
    {
	return 0;
    }
}


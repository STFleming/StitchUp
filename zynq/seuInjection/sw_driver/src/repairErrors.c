#include "gos_interface_driver.h"
#include "stdio.h"
#include "stdlib.h"
#include <string.h>
#include "register_map.h"

#define PCAP_PR (1 << 27) //0 = ICAP CFG, 1 = PCAP_CFG
#define CHECK_BIT(var,pos) ((var) & (1<<(pos)))

int main(int argc, char* argv[])
{
    //unlock the system level control register
    volatile void *devcfg_ctrl = getvaddr(0xF8007000);
    volatile void *semdev = getvaddr(0x42C00000);


//    printf("\t\tInitial: 0x%x\n", *((volatile unsigned *)(semdev)+0)); 

    *((volatile unsigned*)(semdev)+3) = 160;
    *((volatile unsigned*)(semdev)+1) = 1;
    *((volatile unsigned*)(semdev)+1) = 0;

    while(!CHECK_BIT(*((volatile unsigned *)(semdev)+0), 2)){ } //Wait till we are in the observation state
    

    printf("Repair: 0x%x\n", *((volatile unsigned *)(semdev)+0)); 
   

    return CHECK_BIT(*((volatile unsigned *)(semdev)+0), 7);
}


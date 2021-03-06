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

    *((volatile unsigned *)devcfg_ctrl) = *((volatile unsigned *)devcfg_ctrl) & ~PCAP_PR;
    *((volatile unsigned *)(semdev)+4) = 1; //set icap_grant to 1

    while(!CHECK_BIT(*((volatile unsigned *)(semdev)+0), 2)){ } //Wait till we are in the observation state 
    
//    printf("\t\tObservation: 0x%x\n", *((volatile unsigned *)(semdev)+0)); 

    //Attempting to enter idle state
    *((volatile unsigned*)(semdev)+3) = 224;
    *((volatile unsigned*)(semdev)+1) = 1;
    *((volatile unsigned*)(semdev)+1) = 0;
    while(CHECK_BIT(*((volatile unsigned *)(semdev)+0), 1) || CHECK_BIT(*((volatile unsigned *)(semdev)+0), 2) || CHECK_BIT(*((volatile unsigned *)(semdev)+0), 3) || CHECK_BIT(*((volatile unsigned *)(semdev)+0), 4) || CHECK_BIT(*((volatile unsigned *)(semdev)+0), 5)){} //Wait till we are in the Idle state.
    //while(CHECK_BIT(*((volatile unsigned *)(semdev)+0), 2)){ }  
//    printf("\t\tEntering Idle State: 0x%x\n", *((volatile unsigned *)(semdev)+0)); 
    
//    printf("SEM Core Initialisation completed, and ready for injection.\n");

    return 0;
}


#include "gos_interface_driver.h"
#include "stdio.h"
#include "stdlib.h"
#include <string.h>
#include "register_map.h"

#define PCAP_PR (1 << 27) //0 = ICAP CFG, 1 = PCAP_CFG
#define CHECK_BIT(var,pos) ((var) & (1<<(pos)))

int main(int argc, char* argv[])
{
    if (argc < 1) { printf("usage: ./injectError <linear address>\n"); return 1;}
    //unlock the system level control register
    volatile void *semdev = getvaddr(0x42C00000);


    unsigned linaddr = atoi(argv[1]); //get the linear address we are injecting the error at

    //Attempting to enter idle state
    *((volatile unsigned*)(semdev)+3) = 224;
    *((volatile unsigned*)(semdev)+1) = 1;
    *((volatile unsigned*)(semdev)+1) = 0;
    while(CHECK_BIT(*((volatile unsigned *)(semdev)+0), 1) || CHECK_BIT(*((volatile unsigned *)(semdev)+0), 2) || CHECK_BIT(*((volatile unsigned *)(semdev)+0), 3) || CHECK_BIT(*((volatile unsigned *)(semdev)+0), 4) || CHECK_BIT(*((volatile unsigned *)(semdev)+0), 5)){} //Wait till we are in the Idle state.
    //printf("\t\tEntering Idle State: 0x%x\n", *((volatile unsigned *)(semdev)+0)); 


    if(CHECK_BIT(*((volatile unsigned *)(semdev)+0), 1) || CHECK_BIT(*((volatile unsigned *)(semdev)+0), 2) || CHECK_BIT(*((volatile unsigned *)(semdev)+0), 3) || CHECK_BIT(*((volatile unsigned *)(semdev)+0), 4) || CHECK_BIT(*((volatile unsigned *)(semdev)+0), 5))
     {
	printf("ERROR: controller was not in the IDLE state. STATUS: %x\n", *((unsigned *)(semdev)+0));

    }
    else
    {
        *((volatile unsigned*)(semdev)+3) = 192;
        *((volatile unsigned*)(semdev)+2) = linaddr;
        *((volatile unsigned*)(semdev)+1) = 1;
        *((volatile unsigned*)(semdev)+1) = 0;
        while(!CHECK_BIT(*((volatile unsigned *)(semdev)+0), 5)){ } //Wait while the device initialises 
//        printf("\t\tLA:%u    Status: 0x%x\n", linaddr, *((unsigned *)(semdev)+0)); 
        while(CHECK_BIT(*((volatile unsigned *)(semdev)+0), 1) || CHECK_BIT(*((volatile unsigned *)(semdev)+0), 2) || CHECK_BIT(*((volatile unsigned *)(semdev)+0), 3) || CHECK_BIT(*((volatile unsigned *)(semdev)+0), 4) || CHECK_BIT(*((volatile unsigned *)(semdev)+0), 5)){} //Wait till we are in the Idle state.
    
   }


    return 0;
}


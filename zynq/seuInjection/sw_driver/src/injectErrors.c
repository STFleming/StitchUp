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
    volatile void *devcfg_ctrl = getvaddr(0xF8007000);
    volatile void *semdev = getvaddr(0x42C00000);


    printf("Initial: 0x%x\n", *((volatile unsigned *)(semdev)+0)); 

    *((volatile unsigned *)devcfg_ctrl) = *((volatile unsigned *)devcfg_ctrl) & ~PCAP_PR;
    *((volatile unsigned *)(semdev)+4) = 1; //set icap_grant to 1

    unsigned linaddr = atoi(argv[1]); //get the linear address we are injecting the error at

    while(CHECK_BIT(*((volatile unsigned *)(semdev)+0), 1)){ } //Wait while the device initialises 
    
    printf("Observation: 0x%x\n", *((volatile unsigned *)(semdev)+0)); 

    //Attempting to enter idle state
    *((volatile unsigned*)(semdev)+3) = 224;
    *((volatile unsigned*)(semdev)+1) = 1;
    *((volatile unsigned*)(semdev)+1) = 0;
    while(CHECK_BIT(*((volatile unsigned *)(semdev)+0), 2)){ } //Wait while the device initialises 
    printf("Entering Idle State: 0x%x\n", *((volatile unsigned *)(semdev)+0)); 


    *((volatile unsigned*)(semdev)+3) = 192;
    *((volatile unsigned*)(semdev)+2) = linaddr;
    *((volatile unsigned*)(semdev)+1) = 1;
    *((volatile unsigned*)(semdev)+1) = 0;
    while(!CHECK_BIT(*((volatile unsigned *)(semdev)+0), 5)){  } //Wait while the device initialises 

    printf("Inject Error: 0x%x\n", *((unsigned *)(semdev)+0)); 

    //Enable the PCAP
    //*((unsigned *)devcfg_ctrl) = *((unsigned *)devcfg_ctrl) | PCAP_MODE;
    //*((volatile unsigned *)int_sys) = value;
    *((volatile unsigned *)(semdev)+4) = 0; //set icap_grant to 0 
    *((volatile unsigned *)devcfg_ctrl) = *((volatile unsigned *)devcfg_ctrl) | PCAP_PR;

	return 0;
}


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

    //Enable the PCAP
    *((volatile unsigned *)(semdev)+4) = 0; //set icap_grant to 0 
    *((volatile unsigned *)devcfg_ctrl) = *((volatile unsigned *)devcfg_ctrl) | PCAP_PR;

	return 0;
}


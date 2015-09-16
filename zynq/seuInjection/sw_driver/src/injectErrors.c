#include "gos_interface_driver.h"
#include "stdio.h"
#include "stdlib.h"
#include "register_map.h"

#define PCAP_PR (1 << 27) //0 = ICAP CFG, 1 = PCAP_CFG
#define PCAP_MODE (1 << 26) //1 = Enable PCAP

int main(int argc, char* argv[])
{
    if (argc < 1) { printf("usage: ./injectError <linear address>\n"); return 1;}
    void *cfg_lock = getvaddr(0xF8000008);
    *((unsigned*)cfg_lock) = 0x0000DF0DU; //Unlock the PCAP configuration

    void *cfg_icap_grant = getvaddr(0xF8000240);
    void *devcfg_ctrl = getvaddr(0xF8007000);
	void *semdev = getvaddr(0x42C00000);
    void *int_sys = getvaddr(0xF800700C);

    *((unsigned *)cfg_icap_grant) = 0x0000000CU; //set icap_grant to 1
    //Disable the PCAP
    *((unsigned *)devcfg_ctrl) = *((unsigned *)devcfg_ctrl) & ~PCAP_PR;
    //*((unsigned *)devcfg_ctrl) = *((unsigned *)devcfg_ctrl) & ~PCAP_MODE;

    unsigned linaddr = atoi(argv[1]); //get the linear address we are injecting the error at

    //printf("%d\n", linaddr);
    unsigned value = *((unsigned *)int_sys);
    //Pulse wait request and reset
    if(*((int *)(semdev)+0) == 0)
    {
        *((int*)(semdev)+3) = 192;
        *((int*)(semdev)+1) = linaddr;

        *((int*)(semdev)+1) = 1;
        *((int*)(semdev)+1) = 0;
    }

    //Enable the PCAP
    *((unsigned *)devcfg_ctrl) = *((unsigned *)devcfg_ctrl) | PCAP_PR;
    //*((unsigned *)devcfg_ctrl) = *((unsigned *)devcfg_ctrl) | PCAP_MODE;
    *((unsigned *)int_sys) = value;
    *((unsigned *)cfg_icap_grant) = 0x0000000EU; //set icap_grant to 0

	return 0;
}


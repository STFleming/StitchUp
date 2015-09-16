#include "gos_interface_driver.h"
#include "stdio.h"
#include "register_map.h"

int main()
{
	void *hlsdev = getvaddr(0x43C00000);
    printf("status: \ts=%d, \twr=%d, \tf=%d, \tr=%d, \tc=%d, \td=%d, \tr=%d\n", *((int*)(hlsdev)),*((int*)(hlsdev)+1),*((int*)(hlsdev)+2),*((int*)(hlsdev)+3),*((int*)(hlsdev)+4), *((int*)(hlsdev)+5), *((int*)(hlsdev)+6));
    *((int*)(hlsdev)+6) = 1;
    printf("status: \ts=%d, \twr=%d, \tf=%d, \tr=%d, \tc=%d, \td=%d, \tr=%d\n", *((int*)(hlsdev)),*((int*)(hlsdev)+1),*((int*)(hlsdev)+2),*((int*)(hlsdev)+3),*((int*)(hlsdev)+4), *((int*)(hlsdev)+5), *((int*)(hlsdev)+6));
    *((int*)(hlsdev)+6) = 0;
    printf("status: \ts=%d, \twr=%d, \tf=%d, \tr=%d, \tc=%d, \td=%d, \tr=%d\n", *((int*)(hlsdev)),*((int*)(hlsdev)+1),*((int*)(hlsdev)+2),*((int*)(hlsdev)+3),*((int*)(hlsdev)+4), *((int*)(hlsdev)+5), *((int*)(hlsdev)+6));
    return 0;
}

#include "gos_interface_driver.h"
#include "stdio.h"
#include "register_map.h"

int main(int argc, char *argv[])
{
	void *hlsdev = getvaddr(0x43C00000);
	char *linearaddr;
	if( argc != 2)
	{
	 	linearaddr = "0";
	}
	else
	{
		linearaddr = argv[1];
	}	

    //Pulse wait request and reset
    *((int*)(hlsdev)+1) = 1;
    *((int*)(hlsdev)+6) = 1;
    *((int*)(hlsdev)+6) = 0;
    *((int*)(hlsdev)+1) = 0;

    //Start the op
    *((int*)(hlsdev)) = 3;
    *((int*)(hlsdev)) = 0;

    //block till finish
    while((*((int*)(hlsdev)+2) == 0)){}
    //while(*((int*)(hlsdev)+3) != 46){}

    //check for errors
    if(*((int*)(hlsdev)+4) != 0){
    	printf("%s,%d,%d,%d,%d,%d,%d,%d,%d,ERROR\n", linearaddr, *((int*)(hlsdev)),*((int*)(hlsdev)+1),*((int*)(hlsdev)+2),*((int*)(hlsdev)+3),*((int*)(hlsdev)+4), *((int*)(hlsdev)+5), *((int*)(hlsdev)+6), *((int*)(hlsdev)+7));
        //printf("CFG Error detected\n");
        return 1;
    }

    //printf("Circuit completed successfully ans=%d\n", *((int *)(hlsdev)+3));
    printf("%s,%d,%d,%d,%d,%d,%d,%d,%d,OKAY\n", linearaddr, *((int*)(hlsdev)),*((int*)(hlsdev)+1),*((int*)(hlsdev)+2),*((int*)(hlsdev)+3),*((int*)(hlsdev)+4), *((int*)(hlsdev)+5), *((int*)(hlsdev)+6), *((int*)(hlsdev)+7));
	return 0;
}


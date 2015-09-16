#include "gos_interface_driver.h"
#include <stdint.h>
#include <assert.h>
#include <dirent.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stddef.h>

void answer_question(void *dev_ptr, int signal_offset, int answer)
{
	*((int *)(dev_ptr + signal_offset) + 2) = answer;
	*((int *)(dev_ptr + signal_offset) + 1) = 1;
	*((int *)(dev_ptr + signal_offset) + 1) = 0;
	return;
}

int wait_for_answer(void *dev_ptr, int signal_offset)
{
	while(!*((int *)(dev_ptr + signal_offset) + 1)){ }
	return *((int *)(dev_ptr + signal_offset) + 2);	
}

void wait_for_question(void *dev_ptr, int signal_offset) 
{
	while(!*((int *)(dev_ptr + signal_offset))) {}
	return;
}

void ask_question(void*dev_ptr, int signal_offset)
{
	*((int *)(dev_ptr + signal_offset)) = 1;
	*((int *)(dev_ptr + signal_offset)) = 0;
	return;
}

void *getvaddr(int phys_addr)
{

    void *mapped_base;
    int memfd;

    void *mapped_dev_base;
    off_t dev_base = phys_addr;

    memfd = open("/dev/mem", O_RDWR | O_SYNC); //to open this the program needs to be run as root
        if (memfd == -1) {
        printf("Can't open /dev/mem.\n");
        exit(0);
    }

    // Map one page of memory into user space such that the device is in that page, but it may not
    // be at the start of the page.

    mapped_base = mmap(0, MAP_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, memfd, dev_base & ~MAP_MASK);
        if (mapped_base == (void *) -1) {
        printf("Can't map the memory to user space.\n");
        exit(0);
    }
    // get the address of the device in user space which will be an offset from the base
    // that was mapped as memory is mapped at the start of a page

    mapped_dev_base = mapped_base + (dev_base & MAP_MASK);
    return mapped_dev_base;
}


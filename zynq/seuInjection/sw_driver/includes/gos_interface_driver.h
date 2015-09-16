#ifndef GOS_INTERFACE_DRIVER_H
#define GOS_INTERFACE_DRIVER_H

#define MAP_SIZE 4096UL
#define MAP_MASK (MAP_SIZE - 1)

void *getvaddr(int phys_addr);

void ask_question(void*dev_ptr, int signal_offset);
int wait_for_answer(void *dev_ptr, int signal_offset);

void wait_for_question(void *dev_ptr, int signal_offset);
void answer_question(void *dev_ptr, int signal_offset, int answer);


#endif

#ifndef REGISTER_MAP_H
#define REGISTER_MAP_H


#define DEVICE_BASE_ADDR 0x48000000


#define REG_SIZE 0x04
#define A_OFFSET 0 * REG_SIZE
#define B_OFFSET 3 * REG_SIZE
#define C_OFFSET 6 * REG_SIZE

#define A_Q (A_OFFSET + 0*REG_SIZE)
#define A_A (A_OFFSET + 1*REG_SIZE)
#define A_D (A_OFFSET + 2*REG_SIZE)

#define B_Q (B_OFFSET + 0*REG_SIZE)
#define B_A (B_OFFSET + 1*REG_SIZE)
#define B_D (B_OFFSET + 2*REG_SIZE)

#define C_Q (C_OFFSET + 0*REG_SIZE)
#define C_A (C_OFFSET + 1*REG_SIZE)
#define C_D (C_OFFSET + 2*REG_SIZE)
 


#endif

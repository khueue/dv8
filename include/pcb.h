#ifndef PCB_H
#define PCB_H

#include "int_types.h"
#include "registers.h"

#define PCB_STACK_SIZE 0x4000

typedef struct
{
    uint32_t pid;
    uint32_t priority;
    uint8_t stack[PCB_STACK_SIZE];
    registers_t regs;    
} pcb_t;

#endif

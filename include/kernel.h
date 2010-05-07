#ifndef KERNEL_H
#define KERNEL_H

#include "utils.h"

/*
 * ---------------------------------------------------------------------------
 * Functions.
 * ---------------------------------------------------------------------------
 */

uint32_t
kexec(user_prog_pointer program);

void
    kkill_self(void);
void
kinit(void);

void
kexception(void);

#endif

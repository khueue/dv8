#ifndef EXCEPTION_UART_H
#define EXCEPTION_UART_H

#include "utils.h"
#include "mips4kc.h"

/*
 * ---------------------------------------------------------------------------
 * Functions.
 * ---------------------------------------------------------------------------
 */

void
execute_uart(cause_reg_t cause);

#endif

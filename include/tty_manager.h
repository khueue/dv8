#ifndef TTY_MANAGER_H
#define TTY_MANAGER_H

#include "utils.h"

/*
 * ---------------------------------------------------------------------------
 * Functions.
 * ---------------------------------------------------------------------------
 */
int
tty_manager_has_characters(void);

void
tty_manager_put_char(uint8_t c);

uint8_t
tty_manager_get_char(void);

#endif

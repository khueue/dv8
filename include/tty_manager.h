#ifndef TTY_MANAGER_H
#define TTY_MANAGER_H

#include "utils.h"
#include "pcb.h"

/*
 * ---------------------------------------------------------------------------
 * Functions.
 * ---------------------------------------------------------------------------
 */

void
tty_manager_subscribe_for_input(pcb_t *pcb);

void
tty_manager_unsubscribe_from_input(pcb_t *pcb);

int
tty_manager_has_characters(void);

void
tty_manager_put_char(uint8_t c);

uint8_t
tty_manager_get_char(void);

#endif

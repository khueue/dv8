#include "asm.h"
#include "malta.h"
#include "utils.h"
#include "tty_manager.h"

#include "exception_uart.h"

/*
 * Triggers when we press a key.
 */
void
execute_uart(cause_reg_t cause)
{
    if (cause.field.ip & BIT2)
    {
        /* Read character from tty and give it to the manager. */
        if (tty->lsr.field.dr)
        {
            tty_manager_put_char(tty->rbr);
        }

        /* Display the character in the console. */
        if (tty->lsr.field.thre && tty_manager_has_characters())
        {
            tty->thr = tty_manager_get_char();

            /* Determine if we should be notified when transmitter becomes idle. */
            tty->ier.field.etbei = tty_manager_has_characters();
        }

        /* Acknowledge UART interrupt. */
        kset_cause(~BIT12, 0);
    }
}

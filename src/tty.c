#include "tty.h"

#define TTY_UART_BASE_ADDRESS   0xb80003f8

static volatile tty_t *const tty = (tty_t *)TTY_UART_BASE_ADDRESS;

void
tty_putc(char c)
{
    // Poll until ready to transmit.
    while (!tty->lsr.thre)
    {
    }

    // Write character to Transmitter Holding Register
    tty->thr = c;
}

void
tty_print_string(const char* text)
{
    while (text[0] != '\0')
    {
        tty_putc(text[0]);
        ++text;
    }
}

void
tty_test(void)
{
    tty_print_string("dv8-OS version 1.1");

    while (1)
    {
    };
}

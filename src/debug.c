#include "asm.h"
#include "debug.h"
#include "malta.h"

#ifndef NDEBUG
void kdebug_putc(char c)
{
  // BUSY wait for transmitter ready
  while (!tty->lsr.field.thre) {}

  // Transmit character
  tty->thr = c;
}

void kdebug_print(const char* text)
{
  while (text[0] != '\0')
  {
    kdebug_putc(text[0]);
    if (text[0] == '\n') kdebug_putc('\r');
    ++text;
  }
}

void kdebug_println(const char* text)
{
  kdebug_print(text);
  kdebug_putc('\n');
  kdebug_putc('\r');
}

void kdebug_printint(int value)
{
  // Recursive C is not very efficient, but the recursive
  // depth is only logarithmic to the value printed so
  // it should be ok.
  if (value < 0)
  {
      kdebug_putc('-');
      value *= -1;
      
    
  }
  if (value < 10)
  {
    kdebug_putc('0' + value);
  }
  else
  {
    kdebug_printint(value / 10);
    kdebug_putc('0' + (value % 10));
  }
}

void kdebug_magic_break(void);

void kdebug_assert_at(int assertion, const char* file, int line)
{
  if (!assertion)
  {
    kdebug_print("DEBUG: assertion failed at line ");
    kdebug_printint(line);
    kdebug_print(" in file '");
    kdebug_print(file);
    kdebug_println("'.");

    /* Stop simulation, if 'magic-break-enable'. */
    kdebug_magic_break();

    while (1)
    {
      // Do nothing, its the only safe bet when the kernel has crashed.
    }
  }
}

/*
 * Blocks and counts. When counted to interval, prints msg and starts over.
 */
void
do_nothing_forever(const char msg[], uint32_t interval)
{
    uint32_t i = 0;
    for (;;)
    {
        if ((++i % interval) == 0)
        {
            kdebug_println(msg);
        }
    }
}

#endif

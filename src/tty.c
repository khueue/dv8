#include "tty.h"

static volatile tty_t* const tty = (tty_t*) 0xb80003f8;

void tty_putc(char c) {
  // Poll until ready to transmit.
  while ( !tty->lsr.thre ) {}		

  // Write character to Transmitter Holding Register
  tty->thr = c;		    
	  
}

void tty_print_string(const char* text) {
  while (text[0] != '\0') {
    tty_putc(text[0]);
    ++text;
  }
}

void tty_test() {
/*  tty_putc('A');*/

  tty_print_string("Dv8-OS version 1.0");

  while (1) {};
}

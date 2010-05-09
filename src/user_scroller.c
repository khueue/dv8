#include "utils.h"
#include "malta.h"

#include "user_scroller.h"

/*
 * ---------------------------------------------------------------------------
 * Functions.
 * ---------------------------------------------------------------------------
 */

void
maltascr(void)
{
    kdebug_println("start of maltascr");

    int strpos = 0;
    int init = 0;

    /* Funkar för alla strängar utom något som BÖRJAR med null */

    uint8_t tomalta[11];
    tomalta[0] = 'b';
    tomalta[1] = 'a';
    tomalta[2] = 'l';
    tomalta[3] = 'l';
    tomalta[4] = 'e';
    tomalta[5] = 'f';
    tomalta[6] = 'j';
    tomalta[7] = 'o';
    tomalta[8] = 'n';
    tomalta[9] = 'g';
    tomalta[10] = 0;

    malta->ledbar.reg = 0xFF;

	int done;
    int count = 0;

    while(1) {
        if ((count++ % 500000) == 0)
        {
            if((init > 0) && (tomalta[init-1] == 0))
            {
                init = 0;
            }

            strpos = init++;

            for(done = 0; done < 8; done++)
            {
                if(tomalta[strpos] == 0)
                {
                    strpos = 0;
                    malta->asciipos[done++].value = 32;
                }

                if(done < 8) {
                    malta->asciipos[done].value = tomalta[strpos++];
                }
            }

        }

        /* Done printing to malta, jump back to scheduler ...somehow*/
    }
}

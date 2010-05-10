#include "utils.h"
#include "malta.h"
#include "kernel_api.h"
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

    uint8_t tomalta[] = "Kernel Panic ***";

    malta->ledbar.reg = 0xFF;

	int done;

    while(1) {
    
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

        /* Done printing to malta, jump back to scheduler ...somehow*/
        sleep(5000);
    }
}

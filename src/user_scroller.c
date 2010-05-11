#include "utils.h"
#include "malta.h"
#include "kernel_api.h"
#include "user_scroller.h"
#include "settings.h"

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

    uint8_t tomalta[MAX_MESSAGE_LENGTH];
    tomalta[] = DEFAULT_MALTA_TEXT;

    malta->ledbar.reg = 0xFF;

	int done;

    while(1) {
        /* Kolla inbox efter meddelanden, ladda isf in meddelande i tomalta[] */
        
        /* Sen kör vi! */
        if (tomalta[0] != 0)
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

                if(done < 8) 
                {
                    malta->asciipos[done].value = tomalta[strpos++];
                }
            }
        }
        else
        {
            for (done = 0; done < 8; done++)
            {
                malta->ascipos[done].value = 0;
            }
        }
        sleep(MALTA_SLEEP_TIME);
    }
}

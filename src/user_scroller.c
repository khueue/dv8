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
    msg_t *msg = msg_alloc();
    int strpos = 0;
    int init = 0;
    int done;

    uint8_t tomalta[MESSAGE_MAX_LENGTH];
    strcpy(tomalta, DEFAULT_MALTA_TEXT);

    malta->ledbar.reg = 0xFF;

    while (1)
    {
        read_message_by_type(msg, MSG_TYPE_ARGUMENT, MALTA_SLEEP_TIME);
        if (msg_type_is_argument(msg))
        {
            strcpy(ZERO_ARRAY(tomalta), msg_data_get_string(msg));
        }

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
                malta->asciipos[done].value = 0;
            }
        }
    }
}

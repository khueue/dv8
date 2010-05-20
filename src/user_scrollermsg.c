#include "utils.h"
#include "kernel_api.h"

#include "kernel.h" /* SUPER REMOVE XXXXXXXXX */
#include "msg.h"

#include "user_scrollermsg.h"

/*
 * ---------------------------------------------------------------------------
 * Functions.
 * ---------------------------------------------------------------------------
 */

void
scrollermsg(void)
{
    msg_t msg_struct;
    msg_t *msg = &msg_struct;
    uint32_t scroller_pid = 0;
    char str[512] = { '\0' };
    char usage[] = "Usage: scrollermsg [str message]";

    read_message_by_type(msg, MSG_TYPE_ARGUMENT, 300);
    if (msg_type_is_invalid(msg))
    {
        print_str(usage);
        return;
    }
    strcpy(str, msg_data_get_string(msg));
    
    scroller_pid = 2; /* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx */

    msg_data_set_string(msg, str);
    msg_set_receiver_pid(msg, scroller_pid);
    if (send_message(msg))
    {
        /* uhhhhhhhhhh XXXXXXX */
    }
}

#include "utils.h"
#include "kernel_api.h"
#include "msg.h"

#include "user_man.h"

static void
jon_old_as_hell(void)
{
    print_strln("");
    print_strln("  _|_|_|  _|_|_|      _|_|    _|_|_|_|_|  _|_|_|_|_|  _|_|_|    _|_|_| ");
    print_strln("_|        _|    _|  _|    _|      _|          _|        _|    _|       ");
    print_strln("_|  _|_|  _|_|_|    _|_|_|_|      _|          _|        _|      _|_|   ");
    print_strln("_|    _|  _|    _|  _|    _|      _|          _|        _|          _| ");
    print_strln("  _|_|_|  _|    _|  _|    _|      _|          _|      _|_|_|  _|_|_|   ");
    print_strln("                                                                       ");
    print_strln("                                                                       ");
    print_strln("                                                                       ");
    print_strln("                        _|    _|_|    _|      _|  _|                   ");
    print_strln("                        _|  _|    _|  _|_|    _|  _|                   ");
    print_strln("                        _|  _|    _|  _|  _|  _|  _|                   ");
    print_strln("                  _|    _|  _|    _|  _|    _|_|                       ");
    print_strln("                    _|_|      _|_|    _|      _|  _|                   ");
    print_strln("");
}

void
man(void)
{
    msg_t msg_struct;
    msg_t *msg = &msg_struct;
    char arg[256];    
    
    read_message_by_type(msg, MSG_TYPE_ARGUMENT, 330);    
    if (msg_type_is_invalid(msg))
    {
        jon_old_as_hell();
        return;
    }
    
    strcpy(arg,msg_data_get_string(msg));
    
    if (0 == (strcmp("fib", arg)))
    {
        print_strln("Fib workes like this");
    }
    else
    {
        jon_old_as_hell();
        return;
    }
}

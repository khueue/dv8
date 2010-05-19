#include "utils.h"
#include "kernel_api.h"
#include "kernel.h"
#include "msg.h"
#include "user_philo.h"

static int
get_arg(msg_t *msg_get, int time)
{
    read_message_by_type(msg_get, MSG_TYPE_ARGUMENT, time);

    if (msg_type_is_invalid(msg_get) || !msg_data_is_integer(msg_get))
    {

        return -1;
    }

    /* print_int(getpid());
    print_str(" <- ");
    print_int(r);
    print_str(" - ");
    print_int(msg_get_sender_pid(msg_get));
    print_strln(""); */
    return msg_data_get_integer(msg_get);
}

static void
set_arg(msg_t *msg_set, int pid, int val)
{
   /* print_int(getpid());
    print_str(" - ");
    print_int(val);
    print_str(" -> ");
    print_int(pid);
    print_strln(""); */

    msg_type_set_argument(msg_set);
    msg_set_receiver_pid(msg_set, pid);
    msg_data_set_integer(msg_set, val);
    send_message(msg_set);
}

void
philosopher(void)
{
    int table_pid = 0;
    int id = 0;
    int i;
    int max_duration = 4000;
    int min_duration = 1500;
    int status;
    char usage[] = "Error: philo is executed with 'dp'";

    msg_t *msg_set = msg_alloc();
    msg_t *msg_get = msg_alloc();

    /* Get table seat */
    id = get_arg(msg_get,300);

    /* Get table pid */
    table_pid = get_arg(msg_get,300);

    if (id == -1 || table_pid == -1)
    {
        print_strln(usage);
        msg_set = msg_free(msg_set);
        msg_get = msg_free(msg_get);
        return;
    }

    for (i = 0; i < 5; i++)
    {
        print_int(id);
        print_strln(" thinks.");
        sleep(rand_between(min_duration, max_duration));

        print_int(id);
        print_strln(" is hungry.");

        if (id%2 == 0)
        {
            print_int(id);
            print_strln(" tries to get right chopstick.");
            set_arg(msg_set, table_pid, id * 10 + CMD_RIGHT_CHOPSTICK); //first, right
            status = get_arg(msg_get,0);

            print_int(id);
            print_strln(" tries to get left chopstick.");
            set_arg(msg_set, table_pid, id * 10 + CMD_LEFT_CHOPSTICK); //then, left
            status = get_arg(msg_get,0);
        }
        else
        {

            print_int(id);
            print_strln(" tries to get left chopstick.");
            set_arg(msg_set, table_pid, id * 10 + CMD_LEFT_CHOPSTICK); //first, left
            status = get_arg(msg_get, 0);

            print_int(id);
            print_strln(" tries to get right chopstick.");
            set_arg(msg_set, table_pid, id * 10 + CMD_RIGHT_CHOPSTICK); //then, right
            status = get_arg(msg_get,0);
        }

        print_int(id);
        print_strln(" eats.");
        sleep(rand_between(min_duration, max_duration));

        print_int(id);
        print_strln(" puts the chopsticks back");
        set_arg(msg_set, table_pid, id * 10 + CMD_PUT_BACK_CHOPSTICKS);
    }

    print_int(id);
    print_strln(" left table.");
    set_arg(msg_set, table_pid, id * 10 + CMD_LEAVE_TABLE);

    msg_set = msg_free(msg_set);
    msg_get = msg_free(msg_get);
}

void
dphilo_init(void)
{

    int i;
    int nb_philo = NUM_PHILO; //the number of philosophers

    int philosophers_pid[NUM_PHILO];
    int chopsticks[NUM_PHILO];
    int waiting_for_chopstick[NUM_PHILO];

    int philosophers_active = 0;

    msg_t *msg_set = msg_alloc();
    msg_t *msg_get = msg_alloc();

    print_strln("Dining Philosophers");

    msg_type_set_argument(msg_set);

    //creates the chopsticks
    for (i = 0; i < nb_philo; i++)
    {
        chopsticks[i] = 1;
        waiting_for_chopstick[i] = 0;
    }

    //init the philosophers
    for (i = 0; i < nb_philo; i++)
    {
        philosophers_pid[i] = exec("philo", PROCESS_DEFAULT_PRIORITY);
        if (!philosophers_pid[i])
        {
            print_strln("A philosopher did not show up at the dinner...");
        }
        else
        {
            /* send index */
            set_arg(msg_set, philosophers_pid[i], i);

            /* send table process pid */
            set_arg(msg_set, philosophers_pid[i], getpid());

            ++philosophers_active;
        }
    }

    //Wait for messages
    while (1)
    {
        /* arg / 10 = index
           arg % 10 = command */
        int index;
        int command;
        int arg;

        arg = get_arg(msg_get, 0); /* NO TIME OUT XXXX */
        index = arg/10;
        command = arg%10;

        if (command == CMD_LEFT_CHOPSTICK) /* Get left chopstick */
        {
            if (chopsticks[index])
            {
                chopsticks[index] = 0;
                set_arg(msg_set, philosophers_pid[index], 777);

            }
            else if (waiting_for_chopstick[index] == 0)
            {
                waiting_for_chopstick[index] = philosophers_pid[index];
            }
            else
            {
                print_strln("************************* should not get here left!");
            }
        }
        else if (command == CMD_RIGHT_CHOPSTICK) /* Get right chopstick */
        {
            i = (index-1)%nb_philo;
            if (index == 0) /* ??? */
                i = nb_philo-1;

            if (chopsticks[i])
            {
                chopsticks[i] = 0;
                set_arg(msg_set, philosophers_pid[index], 888);
            }
            else if (waiting_for_chopstick[i] == 0)
            {
                waiting_for_chopstick[i] = philosophers_pid[index];
            }
            else
            {
                print_strln("************************* should not get here right!");
            }
        }
        else if (command == CMD_PUT_BACK_CHOPSTICKS) /* Put chopsticks back */
        {
            i = (index-1)%nb_philo;
            if (index == 0) /* ??? */
                i = nb_philo-1;

            if (waiting_for_chopstick[i])
            {
                int pid = waiting_for_chopstick[i];
                waiting_for_chopstick[i] = 0;
                set_arg(msg_set, pid, 999);
            }
            else
            {
                chopsticks[i] = 1;
            }

            if (waiting_for_chopstick[index])
            {
                int pid = waiting_for_chopstick[index];
                waiting_for_chopstick[index] = 0;
                set_arg(msg_set, pid, 666);
            }
            else
            {
                chopsticks[index] = 1;
            }
        }
        else if (command == CMD_LEAVE_TABLE)
        {
            if (!(--philosophers_active))
            {
                break;
            }
        }
        else
        {
            print_strln("******************************************** NEVER");
        }
    }

    msg_set = msg_free(msg_set);
    msg_get = msg_free(msg_get);

    print_strln("The dinner is over");
}

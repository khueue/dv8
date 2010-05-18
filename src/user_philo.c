#include "utils.h"
#include "kernel_api.h"
#include "kernel.h"
#include "msg.h"
#include "user_philo.h"

/*
void
get_chopsticks(int i){
    //this function locks the chopsticks the philosopher i will use
    if (i%2 == 0) {
        pthread_mutex_lock(&chopsticks[(i+1)  % nb_philo]); //first, right
        pthread_mutex_lock(&chopsticks[i]); //then, left
    }
    else
    {
        pthread_mutex_lock(&chopsticks[i]); // first, left
        pthread_mutex_lock(&chopsticks[(i+1) % nb_philo]); //then, right
    }
}

void
put_chopsticks_back(int i)
{
    //this function simply unlocks the mutex on the chopsticks the philosopher i was having
    pthread_mutex_unlock(&chopsticks[i]);
    pthread_mutex_unlock(&chopsticks[(i+1) % nb_philo]);
} */

void
philosopher(void)
{
    int table_pid = 0;
    int id = 0;
    int i;
    int duration = 3;
    
    msg_t *msg_set = msg_alloc();
    msg_t *msg_get = msg_alloc();
    
    /* Get table seat */
    read_message_by_type(msg_get, MSG_TYPE_ARGUMENT, 4000);
    if (msg_type_is_invalid(msg_get))
    {
        print_str("INVALID ARG\n");
        msg_get = msg_free(msg_get);
        return;
    }

    if (msg_data_is_integer(msg_get))
    {
        id = msg_data_get_integer(msg_get);
    }
    else
    {
        print_strln("INVALID ARG - Expected Integer");
        msg_get = msg_free(msg_get);
        return;
    }

    /* Get table pid */
    read_message_by_type(msg_get, MSG_TYPE_ARGUMENT, 4000);
    if (msg_type_is_invalid(msg_get))
    {
        print_str("INVALID ARG\n");
        msg_get = msg_free(msg_get);
        return;
    }

    if (msg_data_is_integer(msg_get))
    {
        table_pid = msg_data_get_integer(msg_get);
    }
    else
    {
        print_strln("INVALID ARG - Expected Integer");
        msg_get = msg_free(msg_get);
        return;
    }

    for(i = 0; i < 10; i++)
    {
        print_int(id);
        print_strln(" thinks.\n");
        sleep(duration);

        print_int(id);
        print_strln(" is hungry and tries to get the chopsticks.");
        //get_chopsticks(id);
        
        if (id%2 == 0) {
            //pthread_mutex_lock(&chopsticks[(i+1)  % nb_philo]); //first, right
            msg_set_receiver_pid(msg_set, table_pid);
            msg_data_set_integer(msg_set, id * 10 + 1);
            send_message(msg_set);
            
            read_message_by_type(msg_get, MSG_TYPE_ARGUMENT, 0);
            if (msg_type_is_invalid(msg_get))
            {
                print_str("INVALID ARG\n");
                msg_get = msg_free(msg_get);
                return;
            }
        
            if (!msg_data_is_integer(msg_get))
            {
                print_strln("INVALID ARG - Expected Integer");
                msg_get = msg_free(msg_get);
                return;
            }
            
            
            //pthread_mutex_lock(&chopsticks[i]); //then, left
            msg_set_receiver_pid(msg_set, table_pid);
            msg_data_set_integer(msg_set, id * 10 + 0);
            send_message(msg_set);
            
            read_message_by_type(msg_get, MSG_TYPE_ARGUMENT, 0);
            if (msg_type_is_invalid(msg_get))
            {
                print_str("INVALID ARG\n");
                msg_get = msg_free(msg_get);
                return;
            }
        
            if (!msg_data_is_integer(msg_get))
            {
                print_strln("INVALID ARG - Expected Integer");
                msg_get = msg_free(msg_get);
                return;
            }
        }
        else
        {
            //pthread_mutex_lock(&chopsticks[i]); // first, left
            //pthread_mutex_lock(&chopsticks[(i+1) % nb_philo]); //then, right
                        //pthread_mutex_lock(&chopsticks[(i+1)  % nb_philo]); //first, left
            msg_set_receiver_pid(msg_set, table_pid);
            msg_data_set_integer(msg_set, id * 10 + 0);
            send_message(msg_set);
            
            read_message_by_type(msg_get, MSG_TYPE_ARGUMENT, 0);
            if (msg_type_is_invalid(msg_get))
            {
                print_str("INVALID ARG\n");
                msg_get = msg_free(msg_get);
                return;
            }
        
            if (!msg_data_is_integer(msg_get))
            {
                print_strln("INVALID ARG - Expected Integer");
                msg_get = msg_free(msg_get);
                return;
            }
            
            
            //pthread_mutex_lock(&chopsticks[i]); //then, right
            msg_set_receiver_pid(msg_set, table_pid);
            msg_data_set_integer(msg_set, id * 10 + 1);
            send_message(msg_set);
            
            read_message_by_type(msg_get, MSG_TYPE_ARGUMENT, 0);
            if (msg_type_is_invalid(msg_get))
            {
                print_str("INVALID ARG\n");
                msg_get = msg_free(msg_get);
                return;
            }
        
            if (!msg_data_is_integer(msg_get))
            {
                print_strln("INVALID ARG - Expected Integer");
                msg_get = msg_free(msg_get);
                return;
            }
        }

        print_int(id);
        print_strln(" eats.");
        sleep(duration);

        print_int(id);
        print_strln(" puts the chopsticks back");
        //put_chopsticks_back(id); //simply puts the chopsticks back on the table
        
        msg_set_receiver_pid(msg_set, table_pid);
        msg_data_set_integer(msg_set, id * 10 + 2);
        send_message(msg_set);
    }
    
    print_int(id);
    print_strln(" left table.");
    //put_chopsticks_back(id); //simply puts the chopsticks back on the table
    
    msg_set_receiver_pid(msg_set, table_pid);
    msg_data_set_integer(msg_set, id * 10 + 3);
    send_message(msg_set);
}

void
dphilo_init(void)
{

    int i;
    int nb_philo = 5; //the number of philosophers

    int philosophers_pid[5];
    int chopsticks[5];
    int waiting_for_chopstick[5];

    int philosophers_active = 0;

    msg_t *msg_set = msg_alloc();
    msg_t *msg_get = msg_alloc();

    //creates the chopsticks
    for(i = 0; i < nb_philo; i++)
    {
        chopsticks[i] = 1;
        waiting_for_chopstick[i] = 0;
    }

    //init the philosophers
    for(i = 0; i < nb_philo; i++)
    {
        philosophers_pid[i] = exec("philo", PROCESS_DEFAULT_PRIORITY);
        if(!philosophers_pid[i])
        {
            print_strln("A philosopher did not show up at the dinner...");
        }
        else
        {
            /* send index */
            msg_set_receiver_pid(msg_set, philosophers_pid[i]);
            msg_data_set_integer(msg_set, i);
            send_message(msg_set);

            /* send table process pid */
            msg_set_receiver_pid(msg_set, philosophers_pid[i]);
            msg_data_set_integer(msg_set, getpid());
            send_message(msg_set);

            ++philosophers_active;
        }
    }

    //Wait for messages
    while(1)
    {
        /* arg / 10 = index
           arg % 10 = command */
        int index;
        int command;
        int arg;
        read_message_by_type(msg_get, MSG_TYPE_ARGUMENT, 4000);
        if (msg_type_is_invalid(msg_get))
        {
            print_str("INVALID ARG\n");
            msg_get = msg_free(msg_get);
            return;
        }

        if (msg_data_is_integer(msg_get))
        {
            arg = msg_data_get_integer(msg_get);
        }
        else
        {
            print_strln("INVALID ARG - Expected Integer");
            msg_get = msg_free(msg_get);
            return;
        }

        index = arg/10;
        command = arg%10;

        if (command == 0) /* Get left chopstick */
        {
            if(chopsticks[index])
            {
                chopsticks[index] = 0;

                msg_set_receiver_pid(msg_set, philosophers_pid[index]);
                msg_data_set_integer(msg_set, 11);
                send_message(msg_set);
            }
            else
            {
                waiting_for_chopstick[i] = philosophers_pid[index];
            }
        }
        else if (command == 1) /* Get right chopstick */
        {
            i = (index-1)%nb_philo;
            if (chopsticks[i])
            {
                chopsticks[i] = 0;

                msg_set_receiver_pid(msg_set, philosophers_pid[i]);
                msg_data_set_integer(msg_set, 10);
                send_message(msg_set);
            }
            else
            {
                waiting_for_chopstick[i] = philosophers_pid[i];
            }
        }
        else if (command == 2) /* Put chopsticks back */
        {
            i = (index-1)%nb_philo;

            chopsticks[i] = 1;
            if (waiting_for_chopstick[i])
            {
                msg_set_receiver_pid(msg_set, waiting_for_chopstick[i]);
                msg_data_set_integer(msg_set, 12);
                send_message(msg_set);

                waiting_for_chopstick[i] = 0;
            }

            chopsticks[index] = 1;
            if(waiting_for_chopstick[index])
            {
                msg_set_receiver_pid(msg_set, waiting_for_chopstick[index]);
                msg_data_set_integer(msg_set, 13);
                send_message(msg_set);

                waiting_for_chopstick[index] = 0;
            }

        }
        else if (command == 3) /* Leave table */
        {
            if(!(--philosophers_active))
            {
                break;
            }
        }
    }

    msg_set = msg_free(msg_set);
    msg_get = msg_free(msg_get);
}

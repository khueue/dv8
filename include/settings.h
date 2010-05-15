#ifndef SETTINGS_H
#define SETTINGS_H

/*
 * ---------------------------------------------------------------------------
 * Constants.
 * ---------------------------------------------------------------------------
 */

/*
 * XXXXXX
 */
#ifndef MS_TO_NEXT_TIMER_INTERRUPT
#define MS_TO_NEXT_TIMER_INTERRUPT 50
#endif

/*
 * Maximum number of PCBs available in the system.
 */
#ifndef NUM_PCBS
#define NUM_PCBS 20
#endif

/*
 * XXXXXXX
 */
#ifndef NUM_MESSAGES
#define NUM_MESSAGES 1024
#endif

/*
 * Maximum number of list nodes available in the system.
 */
#ifndef NUM_LIST_NODES
#define NUM_LIST_NODES 1024
#endif

/*
 * XXXXXXX
 */
#ifndef PROCESS_STACK_SIZE
#define PROCESS_STACK_SIZE 0x4000
#endif

/*
 * XXXXXXXX
 */
#ifndef GLOBAL_DATA_OFFSET
#define GLOBAL_DATA_OFFSET 0x80000000
#endif

/*
 * XXXXXXXXXXXX
 */
#ifndef TTY_FIFO_SIZE
#define TTY_FIFO_SIZE 8
#endif

/*
 * XXXXXXXX
 */
#ifndef STR_BUF_SIZE
#define STR_BUF_SIZE 1024
#endif

/*
 * XXXXX
 */
#ifndef INIT_STACK_SIZE
#define INIT_STACK_SIZE 0x4000
#endif

/*
 * XXXXXXXX
 */
#ifndef EXCN_STACK_SIZE
#define EXCN_STACK_SIZE 0x4000
#endif

#ifndef PROCESS_DEFAULT_PRIORITY 
#define PROCESS_DEFAULT_PRIORITY 100
#endif

#ifndef MALTA_SLEEP_TIME
#define MALTA_SLEEP_TIME 500
#endif

#ifndef DEFAULT_MALTA_TEXT
#define DEFAULT_MALTA_TEXT "Kernel Panic ***"
#endif

#ifndef MESSAGE_MAX_LENGTH
#define MESSAGE_MAX_LENGTH 1024
#endif

#endif

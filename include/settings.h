#ifndef SETTINGS_H
#define SETTINGS_H

/*
 * ---------------------------------------------------------------------------
 * Constants.
 * ---------------------------------------------------------------------------
 */

/*
 * Time between timer interrupts.
 */
#ifndef MS_TO_NEXT_TIMER_INTERRUPT
#define MS_TO_NEXT_TIMER_INTERRUPT 40
#endif

/*
 * Maximum number of PCBs available in the system.
 */
#ifndef NUM_PCBS
#define NUM_PCBS 32
#endif

/*
 * Maximum number of dynamically allocated messages in the system.
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
 * Stack size for each process.
 */
#ifndef PROCESS_STACK_SIZE
#define PROCESS_STACK_SIZE 0x4000
#endif

/*
 * Some memory offset.
 */
#ifndef GLOBAL_DATA_OFFSET
#define GLOBAL_DATA_OFFSET 0x80000000
#endif

/*
 * For the internal TTY bfifo buffer.
 */
#ifndef TTY_FIFO_SIZE
#define TTY_FIFO_SIZE 8
#endif

/*
 * Maximum allowed length for console input.
 */
#ifndef CONSOLE_INPUT_MAX_LENGTH
#define CONSOLE_INPUT_MAX_LENGTH 1024
#endif

/*
 * Kernel stack size. 
 */
#ifndef INIT_STACK_SIZE
#define INIT_STACK_SIZE 0x4000
#endif

/*
 * Initial exception stack size.
 */
#ifndef EXCN_STACK_SIZE
#define EXCN_STACK_SIZE 0x4000
#endif

/*
 * Default priority of newly spawned processes.
 */
#ifndef PROCESS_DEFAULT_PRIORITY 
#define PROCESS_DEFAULT_PRIORITY 100
#endif

/*
 * Scroller update interval.
 */
#ifndef MALTA_SLEEP_TIME
#define MALTA_SLEEP_TIME 100
#endif

/*
 * Scroller text to display as default.
 */
#ifndef DEFAULT_MALTA_TEXT
#define DEFAULT_MALTA_TEXT "Kernel Panic ***"
#endif

#endif

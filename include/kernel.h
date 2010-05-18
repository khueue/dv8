#ifndef KERNEL_H
#define KERNEL_H

#include "utils.h"
#include "pcb.h"
#include "msg.h"

/*
 * XXXXXXx
 */
typedef struct program program_t;
struct program
{
    char name[256];
    user_program_pointer func;
};

/*
 * ---------------------------------------------------------------------------
 * Functions.
 * ---------------------------------------------------------------------------
 */

void
restore_process_state(const pcb_t *pcb);

uint32_t
kexec(const char program[], uint32_t priority);

uint32_t
kgetpid(void);

uint32_t
getpid(void);

void
kprint_char(char c);

void
kprint_str(const char str[]);

void
kprint_strln(const char str[]);

void
kprint_int(int x);

void
kkill_self(void);

uint32_t
kkill(uint32_t pid);

void
kinit(void);

void
kexception(void);

void
ksleep(int time);

uint32_t
kread_message_by_type(msg_t *msg, msg_type_t type, int max_wait_ms);

uint32_t
kread_next_message(msg_t *msg, int max_wait_ms);

#if 0
msg_t *
kread_from_console(void);
#endif

uint32_t
ksend_message(msg_t *msg);

uint32_t
kchange_priority(uint32_t pid, uint32_t priority);

uint32_t
kblock(uint32_t pid);

uint32_t
kunblock(uint32_t pid);

void
kblock_self(void);

#endif

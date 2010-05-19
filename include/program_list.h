#ifndef PROGRAM_LIST_H
#define PROGRAM_LIST_H

/*
 * ---------------------------------------------------------------------------
 * Types.
 * ---------------------------------------------------------------------------
 */

/*
 * Information about a user program.
 */
typedef struct program_info_ program_info_t;

/*
 * ---------------------------------------------------------------------------
 * Functions.
 * ---------------------------------------------------------------------------
 */

/*
 * Returns a program's point of entry function, or NULL if no matching program
 * is found in the program list.
 */
user_program_pointer
program_list_get_program_code(const char program_name[]);


void
program_list_print(void);

/*
 * Returns true if a program is allowed to be executed by a user through the
 * shell, otherwise false.
 */
int
program_list_program_executable_by_shell(const program_info_t *program);

#endif

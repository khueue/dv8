#ifndef PROGRAM_LIST_H
#define PROGRAM_LIST_H

#include "utils.h"

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

/*
 * Resets the iterator.
 */
void
program_list_iter_reset(void);

/*
 * Returns true if there are more uniterated programs in the iterator.
 */
int
program_list_iter_has_next(void);

/*
 * Returns the next uniterated item in the iterator.
 */
const program_info_t *
program_list_iter_next(void);

/*
 * Prints a list of all shell-visible programs.
 */
void
program_list_print(void);

int
program_list_name_exist(char *name);

/*
 * Returns the name of a program.
 */
const char *
program_info_get_name(const program_info_t *program);

/*
 * Returns the code pointer for a program.
 */
user_program_pointer
program_info_get_code(const program_info_t *program);

/*
 * Returns true if a program is allowed to be executed by a user through the
 * shell.
 */
int
program_info_is_executable_by_shell(const program_info_t *program);

#endif

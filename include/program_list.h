#ifndef PROGRAM_LIST_H
#define PROGRAM_LIST_H

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

#endif

# Code Convention Suggestions

All of this is of course up for debate, but here is an outline of what I feel
is a reasonable code convention for C code. I've included arguments where
appropriate. I think.

## Indentation

A tab is __exactly four spaces__. So no hard tabs (\t) anywhere.

## Nesting

Several levels of nesting is difficult to understand. If you find yourself
in a _for_ in an _if_ in a _for_ -- refactor! Break it up into smaller
functions with descriptive names.

## Comments

If you feel the need to write a comment to explain a piece of code -- think
twice. Maybe you could break things up into smaller functions, with better
names, etc.? See the previous section on Nesting.

Comments are nice, but only if they convey something that the code simply
cannot.

Single-line comments (no slash-slash in C, that's C++!):

    /* Pretty much like this. */
    int x ...;

Multi-line comments:

    /*
     * The code below ... ZOMG!
     * Watch out!
     */
    for (...)
    {
        ... really complicated code ... ouch ...
    }

## Braces

Braces __always__ line up in the same column. This makes it easier to quickly
associate matching pairs:

    struct pcb
    {
        uint32_t priority;
    };

    pcb_t *
    alloc_pcb(void)
    {
        if (...)
        {
            ...
        }
    }

Prefer brackets over no brackets. This makes it easier to maintain code. Do
this:

    if (...)
    {
        ...
    }
    else
    {
        ...
    }

instead of:

    if (...)
        ...
    else
        ...

## Language Constructs

Since language constructs aren't function calls, they should look a little
different (also read the section on Function Declarations), so keep a single
space after the keyword, just before any parentheses:

    if (...)
    {
        ...
    }

    while (...)
    {
        ...
    }

    for (i = 0; i < 10; ++i)
    {
        ...
    }

## Structs

Declaring a struct involves three steps: a "forward declaration", a typedef
and the declaration itself. The forward declaration lets us to use the symbol
before it is actually defined. The typedef (which should end in '_t') lets us
avoid a few strokes, which might lead to cleaner (or at least shorter) code.
Here goes:

    struct pcb;
    typedef struct pcb pcb_t;
    struct pcb
    {
        pcb_t *next;
    };

## Functions

### Function Declarations

Just like with global variable declarations, keep a newline just before the
function name, and avoid spaces between the name and the argument list:

    pcb_t *
    alloc_pcb(void);

    void
    free_pcb(pcb_t *pcb);

    void
    init_pcb_freelist(void);

As you can see, it's easy to quickly scan the function names, since they are
all collected in the leftmost column.

If a functions takes no arguments, then it should be declared (and defined)
as taking void. A function that is typed to nothing, not even void, is assumed
to take a variable number of arguments.

## Ternary If (condition ? on_true : on_false)

Use it only for very concise expressions. __Never__ nest. This might be okay,
but would probably look better as a regular if:

    int
    fib(int n)
    {
        return (i <= 1) ? 0 : fib(n-1)+fib(n-2);
    }

## Variables


As for pointers, always place the asterisk (*) to the right (if there is
choice in the matter):

    pcb_t *pcb = ...;

    pcb_t *
    alloc_pcb(void);

Why, you say? Isn't the pointer part of the type? No, not really. Consider
this (which is awful, and should be avoided, no less because of this reason):

    int* p1, p2;

Did you expect both p1 and p2 to be pointers to int? Too bad. Only p1 is a
pointer. With better formatting, the code looks this:

    int *p1, p2;

It's now much more obvious that only p1 is a pointer. So how do we turn p2
into a pointer then? The best way is of course to break it out, which in the
long run also makes maintenance easier:

    int *p1;
    int *p2;

But that's not the answer you were looking for, now is it. This is how you do
it the obscure way:

    int *p1, *p2;

### Local Variables

Variable declarations must appear __first__ in a block of braces (this is
actually required by ANSI C). Also try to keep variables as local as possible
to their usage:

    if (...)
    {
        size_t i;
        for (i = 0; ...)
        {
            char c = getchar();
            putchar(c);
        }
    }

### Global Variables

If you declare variables on the top-level (outside of any functions), they are
accessible throughout the entire file. If they are not mentioned in a header
file as part of an interface, they should be marked as static, to make them
private to the file they were declared in:

    static pcb_t *
    g_pcb_freelist;

Any variable that is not local to a function (i.e. declared on the top-level)
should be prefixed with the two characters 'g_'. This makes it easy to spot
their usage inside functions.

Since global variables tend to have more or less long storage qualifiers
(static, volatile, const, etc.) it might be nice to have a newline just before
the variable name. This neatly lines up all symbol names in the leftmost
column, which makes them easier to scan:

    static pcb_t *
    g_pcb_freelist;

    static volatile tty_t *const
    tty = (tty_t *)0x...;

### Variable Naming

Use lowercase letters only, and underscores for spaces. As per the previous
section on Global Variables, prefix globals with 'g_'. [Should we perhaps
use some convention for pointers too? Such as 'pcb_t **pp_var' meaning pp_var
is a 'pointer to pointer to pcb_t'.]

Prefer long names over short ones, and avoid non-customary abbreviations. A
simple rule of thumb is "the smaller the scope, the shorter the name". Which
roughly means that globals must have descriptive and possibly long names
(g_pcb_freelist), while short-lived variables, such as loop counters,
should be kept as short and idiomatic as possible (i, j, next).

## Constants

Since constants are pretty much global, always prefer longer names. By
convention, always use UPPERCASE letters and underscores for spaces:

    #ifndef KERNEL_STACK_SIZE
        #define KERNEL_STACK_SIZE 0x4000
    #endif

As you can see, also wrap the #define in an #ifndef which uses the very same
constant. This enables us to set various settings at compile time, for example
in the Makefile, instead of searching around in all the header files. [Maybe
we should keep a list somewhere of all constants that describe system
"settings"?]

If the replacement isn't atomic, wrap it in parentheses (and read the next
section):

    #define BIT3 (1 << 2)

This avoids unnecessary headaches involving operator precedence. Note that
this kind of macro isn't a configurable "setting", so no #ifndef here.

## (Complicated) Macros

__Avoid!__ If you must use them, always wrap the entire expression and all
arguments in parentheses:

    #define MAX(a,b) ( ((a) > (b)) ? (a) : (b) )

If you don't know why you should do this, then avoid macros altogether.

## Header Files

Always use "include guards" to avoid wasteful (and most likely erroneous)
inclusion. For example, in file 'file_1.h', wrap the entire code in:

    #ifndef FILE_1_H
    #define FILE_1_H

    ... file contents here ...

    #endif

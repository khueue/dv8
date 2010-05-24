#include "utils.h"
#include "kernel_api.h"
#include "msg.h"

#include "user_dv8.h"

/*
 * ---------------------------------------------------------------------------
 * Functions.
 * ---------------------------------------------------------------------------
 */

static void
print_strln_center(const char *str)
{
    char out[80];
    int i;
    int pad = (80 - strlen(str)) / 2;

    for(i = 0; i < pad; i++)
    {
        out[i] = ' ';
    }

    while (*str)
    {
        out[i++] = *str++;
    }

    out[i] = '\0';
    print_strln(out);
}

void
dv8(void)
{
    print_strln_center("");
    print_strln_center("888888ba                    oo            dP           ");
    print_strln_center("88    `8b                                 88           ");
    print_strln_center("88     88 .d8888b. dP   .dP dP .d8888b. d8888P .d8888b.");
    print_strln_center("88     88 88ooood8 88   d8' 88 88'  `88   88   88ooood8");
    print_strln_center("88    .8P 88.  ... 88 .88'  88 88.  .88   88   88.  ...");
    print_strln_center("8888888P  `88888P' 8888P'   dP `88888P8   dP   `88888P'");
    print_strln_center("ooooooooooooooooooooooooooooooooooo GROUP DV8 2010 oooo");
    print_strln_center("");
    print_strln_center("By");
    print_strln_center("");
    print_strln_center("Jon");
    print_strln_center("David");
    print_strln_center("Sebastian");
    print_strln_center("Simon");
    print_strln_center("");

}

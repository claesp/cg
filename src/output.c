#include <unistd.h>

#include "cg.h"
#include "output.h"

void
out_draw_rows(void)
{
    int y = 0;
    for (y = 0; y < ECFG.rows; y++) {
        write(STDOUT_FILENO, "~", 1);

        if (y < ECFG.rows - 1) {
            write(STDOUT_FILENO, "\r\n", 2);
        }
    }
}

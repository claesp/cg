#include <unistd.h>

#include "buffer.h"
#include "cg.h"
#include "output.h"

void
out_draw_rows(cgbuf *buf)
{
    int y = 0;
    for (y = 0; y < ECFG.rows; y++) {
		buf_append(buf, "~", 1);

        if (y < ECFG.rows - 1) {
			buf_append(buf, "\r\n", 2);
        }
    }
}

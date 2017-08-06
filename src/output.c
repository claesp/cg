#include <stdio.h>
#include <unistd.h>

#include "buffer.h"
#include "cg.h"
#include "output.h"
#include "screen.h"

void
out_draw_rows(cgbuf *buf)
{
	int y = 0;
	for (y = 0; y < ECFG.rows; y++) {
		out_draw_welcome(buf, y);

		buf_append(buf, SCR_ESC_CLR_LINE, 3);
		if (y < ECFG.rows - 1) {
			buf_append(buf, "\r\n", 2);
		}
	}
}

void
out_draw_welcome(cgbuf *buf, int row)
{
	if (row == ECFG.rows / 2) {
		char w[9];
		int wlen = snprintf(w, sizeof(w), "cg %s", VERSION);
		if (wlen > ECFG.cols)
			wlen = ECFG.cols;

		int p = (ECFG.cols - wlen) / 2;
		if (p) {
			buf_append(buf, "~", 1);
			p--;
		}
		while (p--)
			buf_append(buf, " ", 1);
		buf_append(buf, w, wlen);
	} else {
		buf_append(buf, "~", 1);
	}
}

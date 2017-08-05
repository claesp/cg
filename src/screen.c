#include <stdio.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include "buffer.h"
#include "output.h"
#include "screen.h"

int
scr_get_cursor_pos(int *rows, int *cols)
{
    char buf[32];
    unsigned int i = 0;

    if (write(STDOUT_FILENO, SCR_ESC_CUR_POS, 4) != 4)
        return -1;

    while (i < sizeof(buf) - 1) {
        if (read(STDIN_FILENO, &buf[i], 1) != 1)
            break;
        if (buf[i] == 'R')
            break;
        i++;
    }
    buf[i] = '\0';

    if (buf[0] != '\x1b' || buf[1] != '[')
        return -1;

    if (sscanf(&buf[2], "%d;%d", rows, cols) != 2)
        return -1;

    return 0;
}

int
scr_get_win_size(int *rows, int *cols)
{
    struct winsize ws;

    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1 || ws.ws_col == 0) {
        if (write(STDOUT_FILENO, SCR_ESC_CUR_BR, 12) != 12)
            return -1;
        return scr_get_cursor_pos(rows, cols);
    } else {
        *rows = ws.ws_row;
        *cols = ws.ws_col;
        return 0;
    }
}

void
scr_refresh(void)
{
	cgbuf buf = CGBUF_INIT;

	buf_append(&buf, SCR_ESC_CUR_HIDE, 6);
	buf_append(&buf, SCR_ESC_CLR, 4);
	buf_append(&buf, SCR_ESC_CUR_TL, 3);

    out_draw_rows(&buf);

	buf_append(&buf, SCR_ESC_CUR_TL, 3);
	buf_append(&buf, SCR_ESC_CUR_SHOW, 6);

	write(STDOUT_FILENO, buf.b, buf.len);
	buf_free(&buf);
}

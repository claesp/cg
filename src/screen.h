#ifndef SCREEN_H
#define SCREEN_H

#define ESC_CLR "\x1b[2J"               /* clear screen */
#define ESC_CUR_TL "\x1b[H"             /* move cursor top-left */
#define ESC_CUR_POS "\x1b[6n"           /* get cursor position */
#define ESC_CUR_BR "\x1b[999C\x1b[999B" /* move cursor bottom-right-ish */

int scr_get_cursor_pos(int *rows, int *cols);
int scr_get_win_size(int *rows, int *cols);
#endif

#ifndef SCREEN_H
#define SCREEN_H

#define SCR_ESC_CLR "\x1b[2J"               /* clear screen */
#define SCR_ESC_CUR_BR "\x1b[999C\x1b[999B" /* move cursor bottom-right-ish */
#define SCR_ESC_CUR_HIDE "\x1b[?25l" /* hide cursor */
#define SCR_ESC_CUR_POS "\x1b[6n"           /* get cursor position */
#define SCR_ESC_CUR_SHOW "\x1b[?25h" /* show cursor */
#define SCR_ESC_CUR_TL "\x1b[H"             /* move cursor top-left */

int scr_get_cursor_pos(int *rows, int *cols);
int scr_get_win_size(int *rows, int *cols);
void scr_refresh(void);
#endif

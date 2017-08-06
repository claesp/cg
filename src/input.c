#include <errno.h>
#include <stdlib.h>
#include <unistd.h>

#include "cg.h"
#include "input.h"
#include "screen.h"

#define KEY_CTRL(k) ((k) & 0x1f)
enum keys {
	KEY_ARROW_LEFT = 1000,
	KEY_ARROW_RIGHT,
	KEY_ARROW_UP,
	KEY_ARROW_DOWN,
	KEY_PAGE_UP,
	KEY_PAGE_DOWN
};

void
inp_command(void)
{
	int c = inp_read_key();

	switch (c) {
		case KEY_CTRL('q'):
			write(STDOUT_FILENO, SCR_ESC_CLR, 4);
			write(STDOUT_FILENO, SCR_ESC_CUR_TL, 3);
			exit(0);
			break;
		case KEY_PAGE_UP:
			break;
		case KEY_PAGE_DOWN:
			break;
		case KEY_ARROW_LEFT:
			/* FALLTHROUGH */
		case KEY_ARROW_RIGHT:
			/* FALLTHROUGH */
		case KEY_ARROW_UP:
			/* FALLTHROUGH */
		case KEY_ARROW_DOWN:
			inp_move_cur(c);
			break;
	}
}

void
inp_disable_raw(void)
{
	if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &ECFG.init_term) == -1)
		die("tcsetattr");
}

void
inp_enable_raw(void)
{
	if (tcgetattr(STDIN_FILENO, &ECFG.init_term) == -1)
		die("tcgetattr");
	atexit(inp_disable_raw);
	
	struct termios r = ECFG.init_term;
	r.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
	r.c_oflag &= ~(OPOST);
	r.c_cflag |= (CS8);
	r.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
	r.c_cc[VMIN] = 0;
	r.c_cc[VTIME] = 1;

	if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &r) == -1)
		die("tcsetattr");
}

void
inp_move_cur(int key)
{
	switch (key) {
		case KEY_ARROW_LEFT:
			if (ECFG.cx > 0) {
				ECFG.cx--;
			} else {
				if (ECFG.cy > 0) {
					ECFG.cx = ECFG.cols;
					ECFG.cy--;
				} else {
					ECFG.cx = 0;
				}
			}
			break;
		case KEY_ARROW_RIGHT:
			if (ECFG.cx < ECFG.cols) {
				ECFG.cx++;
			} else {
				if (ECFG.cy != (ECFG.rows - 1)) {
					ECFG.cx = 0;
					ECFG.cy++;
				} else {
					ECFG.cx = ECFG.cols;
				}
			}
			break;
		case KEY_ARROW_UP:
			if (ECFG.cy > 0) {
				ECFG.cy--;
			}
			break;
		case KEY_ARROW_DOWN:
			if (ECFG.cy != (ECFG.rows - 1)) {
				ECFG.cy++;
			}
			break;
	}
}

int
inp_read_key(void)
{
	int n;
	char c;

	while ((n = read(STDIN_FILENO, &c, 1)) != 1) {
		if (n == -1 && errno != EAGAIN)
			die("read");
	}

	if (c == '\x1b') {
		char sq[3];

		if (read(STDIN_FILENO, &sq[0], 1) != 1)
			return '\x1b';
		if (read(STDIN_FILENO, &sq[1], 1) != 1)
			return '\x1b';

		if (sq[0] == '[') {
			if (sq[1] >= '0' && sq[1] <= '9') {
				if (read(STDIN_FILENO, &sq[2], 1) != 1)
					return '\x1b';

				if (sq[2] == '~') {
					switch (sq[1]) {
						case '5':
							return KEY_PAGE_UP;
						case '6':
							return KEY_PAGE_DOWN;
					}
				}
			} else {
				switch (sq[1]) {
					case 'A':
						return KEY_ARROW_UP;
					case 'B':
						return KEY_ARROW_DOWN;
					case 'C':
						return KEY_ARROW_RIGHT;
					case 'D':
						return KEY_ARROW_LEFT;
				}
			}
		}

		return '\x1b';
	} else {
		return c;
	}
}

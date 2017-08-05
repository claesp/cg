#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>

#include "cg.h"

#define KEY_CTRL(k) ((k) & 0x1f)

struct config {
	int rows;
	int cols;
	struct termios init_term;
};

const char *VERSION = "0.0.1";

const char *ESC_CLR     = "\x1b[2J"; /* clear screen */
const char *ESC_CUR_TL  = "\x1b[H";  /* move cursor top-left */
const char *ESC_CUR_POS = "\x1b[6n"; /* get cursor position */

struct config ECFG;

void
die(const char *s)
{
	write(STDOUT_FILENO, ESC_CLR, 4);
	write(STDOUT_FILENO, ESC_CUR_TL, 3);
	perror(s);
	exit(1);
}

void
draw_rows(void)
{
	int y = 0;
	for (y = 0; y < ECFG.rows; y++) {
		write(STDOUT_FILENO, "~", 1);

		if (y < ECFG.rows - 1) {
			write(STDOUT_FILENO, "\r\n", 2);
		}
	}
}

int
get_cursor_pos(int *rows, int *cols)
{
	char buf[32];
	unsigned int i = 0;

	if (write(STDOUT_FILENO, ESC_CUR_POS, 4) != 4)
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
get_win_size(int *rows, int *cols)
{
	struct winsize ws;

	if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1 || ws.ws_col == 0) {
		if (write(STDOUT_FILENO, "\x1b[999C\x1b[999B", 12) != 12)
			return -1;
		return get_cursor_pos(rows, cols);
	} else {
		*rows = ws.ws_row;
		*cols = ws.ws_col;
		return 0;
	}
}

void
init_editor(void)
{
	if (get_win_size(&ECFG.rows, &ECFG.cols) == -1)
		die("get_win_size");
}

void
input(void)
{
	char c = read_key();

	switch (c) {
		case KEY_CTRL('q'):
			write(STDOUT_FILENO, ESC_CLR, 4);
			write(STDOUT_FILENO, ESC_CUR_TL, 3);
			exit(0);
			break;
	}
}

int
main(int argc, char *argv[])
{
	int vflag = 0;
	int c = 0;
	while ((c = getopt(argc, argv, "v")) != -1) {
		switch (c) {
			case 'v':
				vflag = 1;
				break;
			default:
				usage(argv[0]);
		}
	}

	read_loop();

	return 0;
}

void
rawmode_off(void)
{
	if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &ECFG.init_term) == -1)
		die("tcsetattr");
}

void
rawmode_on(void)
{
	if (tcgetattr(STDIN_FILENO, &ECFG.init_term) == -1)
		die("tcgetattr");
	atexit(rawmode_off);

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

char
read_key(void)
{
	int n;
	char c;

	while ((n = read(STDIN_FILENO, &c, 1)) != 1) {
		if (n == -1 && errno != EAGAIN)
			die("read");
	}

	return c;
}

void
read_loop(void)
{
	init_editor();
	rawmode_on();

	while(1) {
		refresh();
		input();
	}
}

void
refresh(void)
{
	write(STDOUT_FILENO, ESC_CLR, 4);
	write(STDOUT_FILENO, ESC_CUR_TL, 3);

	draw_rows();

	write(STDOUT_FILENO, ESC_CUR_TL, 3);
}

void
usage(char *name)
{
	printf("usage: %s [-v]\n", name); 
	exit(0);
}


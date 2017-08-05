#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

#include "cg.h"

#define KEY_CTRL(k) ((k) & 0x1f)

const char *VERSION = "0.0.1";
struct termios bkterm;

void
die(const char *s)
{
	perror(s);
	exit(1);
}

void
input(void)
{
	char c = read_key();

	switch (c) {
		case KEY_CTRL('q'):
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
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &bkterm);
}

void
rawmode_on(void)
{
	if (tcgetattr(STDIN_FILENO, &bkterm) == -1)
		die("tcgetattr");
	atexit(rawmode_off);

	struct termios r = bkterm;
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
	rawmode_on();

	while(1) {
		refresh();
		input();
	}
}

void
refresh(void)
{
	write(STDOUT_FILENO, "\x1b[2J", 4); /* clear screen */
	write(STDOUT_FILENO, "\x1b[H", 3);  /* move cursor to top-left */
}

void
usage(char *name)
{
	printf("usage: %s [-v]\n", name); 
}


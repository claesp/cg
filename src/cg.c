#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

#include "cg.h"

char *VERSION = "0.0.1";
struct termios bkterm;

void
d_rawmode(void)
{
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &bkterm);
}

void
e_rawmode(void)
{
	tcgetattr(STDIN_FILENO, &bkterm);
	atexit(d_rawmode);

	struct termios r = bkterm;
	r.c_iflag &= ~(ICRNL | IXON);
	r.c_oflag &= ~(OPOST);
	r.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &r);
}

void
input(void)
{
	e_rawmode();

	char c;
	while (read(STDIN_FILENO, &c, 1) == 1 && c != 'q') {
		if (iscntrl(c)) {
			printf("%d\r\n", c);
		} else {
			printf("%d ('%c')\r\n", c, c);
		}
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

	input();

	return 0;
}

void
usage(char *name)
{
	printf("usage: %s [-v]\n", name); 
}


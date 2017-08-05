#include <errno.h>
#include <stdlib.h>
#include <unistd.h>

#include "cg.h"
#include "input.h"
#include "screen.h"

#define KEY_CTRL(k) ((k) & 0x1f)

void
inp_command(void)
{
    char c = inp_read_key();

    switch (c) {
        case KEY_CTRL('q'):
            write(STDOUT_FILENO, SCR_ESC_CLR, 4);
            write(STDOUT_FILENO, SCR_ESC_CUR_TL, 3);
            exit(0);
            break;
		case 'w':
			/* FALLTHROUGH */
		case 's':
			/* FALLTHROUGH */
		case 'a':
			/* FALLTHROUGH */
		case 'd':
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
inp_move_cur(char key)
{
	switch (key) {
		case 'a':
			ECFG.cx--;
			break;
		case 'd':
			ECFG.cx++;
			break;
		case 'w':
			ECFG.cy--;
			break;
		case 's':
			ECFG.cy++;
			break;
	}
}

char
inp_read_key(void)
{
    int n;
    char c;

    while ((n = read(STDIN_FILENO, &c, 1)) != 1) {
        if (n == -1 && errno != EAGAIN)
            die("read");
    }

    return c;
}

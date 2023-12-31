#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include "cg.h"
#include "input.h"
#include "output.h"
#include "screen.h"

const char *VERSION = "0.0.1";
struct config ECFG;

void
die(const char *s)
{
	write(STDOUT_FILENO, SCR_ESC_CLR, 4);
	write(STDOUT_FILENO, SCR_ESC_CUR_TL, 3);
	perror(s);
	exit(1);
}

void
edit_loop(void)
{
	init_editor();
	inp_enable_raw();

	while(1) {
		scr_refresh();
		inp_command();
	}
}

void
init_editor(void)
{
	ECFG.cx = 0;
	ECFG.cy = 0;

	if (scr_get_win_size(&ECFG.rows, &ECFG.cols) == -1)
		die("scr_get_win_size");
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

	edit_loop();

	return 0;
}

void
usage(char *name)
{
	printf("usage: %s [-v]\n", name); 
	exit(0);
}


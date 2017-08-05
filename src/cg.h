#ifndef CG_H
#define CG_H
#include <termios.h>

struct config {
	int rows;
	int cols;
	struct termios init_term;
};
extern struct config ECFG;

void die(const char *s);
void draw_rows(void);
void edit_loop(void);
void init_editor(void);
int main(int argc, char *argv[]);
void rawmode_off(void);
void rawmode_on(void);
void refresh(void);
void usage(char *name);
#endif

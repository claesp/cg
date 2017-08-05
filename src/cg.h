#ifndef CG_H
#define CG_H
void die(const char *s);
void draw_rows(void);
void init_editor(void);
void input(void);
int main(int argc, char *argv[]);
void rawmode_off(void);
void rawmode_on(void);
char read_key(void);
void read_loop(void);
void refresh(void);
void usage(char *name);
#endif

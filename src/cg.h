void die(const char *s);
void draw_rows(void);
int get_cursor_pos(int *rows, int *cols);
int get_win_size(int *rows, int *cols);
void init_editor(void);
void input(void);
int main(int argc, char *argv[]);
void rawmode_off(void);
void rawmode_on(void);
char read_key(void);
void read_loop(void);
void refresh(void);
void usage(char *name);

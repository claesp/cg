#ifndef BUFFER_H
#define BUFFER_H
#define CGBUF_INIT {NULL, 0}
typedef struct {
	char *b;
	int len;
} cgbuf;

void buf_append(cgbuf *buf, const char *s, int len);
void buf_free(cgbuf *buf);
#endif

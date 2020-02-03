#include <assert.h>
#include <sys/types.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/fcntl.h>
#include <termios.h>
#include <string.h>
#include <stdlib.h>

typedef struct {
	unsigned char buf[256];
	int n;
	int magic_valid;
} cmd_parser;

int cmd_input(cmd_parser *p, char *c, int n) {
	memcpy(p->buf + p->n, c, n);
	p->n += n;
	if (!p->magic_valid && p->n >= 4) {
		// test magic
		p->magic_valid = 0 == memcmp(p->buf, &(unsigned char[]){0xD1, 53, 0xAE, 53}, 4);
		if (!p->magic_valid) {
			p->n = 0;
		}
	}
#define REQUIRE_BYTES(k) if (p->n < (k)) return (k) - p->n
	REQUIRE_BYTES(4); // besoin de verifier le magic avant de continuer
	REQUIRE_BYTES(5); // besoin de connaitre la taille avant de continuer
	REQUIRE_BYTES(5 + p->buf[4]); // besoin de la commande à executer...
#undef REQUIRE_BYTES
	p->buf[p->n] = 0;
	system((char*)(p->buf + 5));
	p->n = 0;
	return 0;
}

int main(int argc, char *argv[argc])
{
	if (argc != 2)
		return 1;
	int fd = open(argv[1], O_RDONLY);
	if (fd < 0)
		return 1;
	cmd_parser p = {0};
	int toread = 4;
	while (1) {
		char c[toread];
		int r = read(fd, c, toread);
		toread = cmd_input(&p, c, r);
	}
}

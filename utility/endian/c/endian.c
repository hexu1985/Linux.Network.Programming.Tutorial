#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

int
main(int argc, char **argv)
{
	union {
	  uint16_t  s;
      char   c[sizeof(uint16_t)];
    } un;

	un.s = 0x0102;
    system("uname -a");
	if (sizeof(uint16_t) == 2) {
		if (un.c[0] == 1 && un.c[1] == 2)
			printf("big-endian\n");
		else if (un.c[0] == 2 && un.c[1] == 1)
			printf("little-endian\n");
		else
			printf("unknown\n");
	} else
		printf("sizeof(uint16_t) = %d\n", (int) sizeof(uint16_t));

	exit(0);
}

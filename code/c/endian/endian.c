#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

int main( void )
{
	uint32_t x = 0x12345678;	/* 305419896 */
	unsigned char *xp = ( char * )&x;

	printf( "%0x %0x %0x %0x\n",
		xp[ 0 ], xp[ 1 ], xp[ 2 ], xp[ 3 ] );
	exit( 0 );
}

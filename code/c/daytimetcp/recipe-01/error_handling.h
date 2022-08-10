#pragma once

#include <stdio.h>
#include <stdlib.h>

/*------------------------------------------------------------------------
 * error_handling - print an error message and exit
 *------------------------------------------------------------------------
 */
static void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

#pragma once

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

/*------------------------------------------------------------------------
 * error_handling - print an error message and exit
 *------------------------------------------------------------------------
 */
static int error_handling(const char *format, ...)
{
	va_list	args;

	va_start(args, format);
	vfprintf(stderr, format, args);
	va_end(args);
	fputc('\n', stderr);
	exit(1);
}

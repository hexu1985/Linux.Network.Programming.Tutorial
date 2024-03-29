#pragma once

#include <stdio.h>

			/* prototypes for our stdio wrapper functions: see {Sec errors} */
void
Fclose(FILE *);

FILE *
Fdopen(int, const char *);

char *
Fgets(char *, int, FILE *);

FILE *
Fopen(const char *, const char *);

void
Fputs(const char *, FILE *);

FILE *
Popen(const char *, const char *);

int
Pclose(FILE *);


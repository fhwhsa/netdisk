#include "wrap.h"

void perr_exit(const char *s)
{
	perror(s);
	exit(-1);
}

int Printf(const char* format, ...)
{
	va_list args;
	va_start(args, format);
	int res = vprintf(format, args);
	fflush(stdout);
	va_end(args);
	return res;
}

void Perror(const char *s)
{
	perror(s);
	fflush(stderr);
}

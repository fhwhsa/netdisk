/**
 * 包裹函数
*/

#ifndef __WRAP_H_
#define __WRAP_H_
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

void perr_exit(const char *s);
int Printf(const char* format, ...);
void Perror(const char *s);
#endif


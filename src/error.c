#ifndef ERROR_FATAL_H
#define ERROR_FATAL_H


#include <stdarg.h> 
#include <stdio.h>
#include <stdlib.h>

void Fatal(const char *err) {
  fprintf(stderr, "\n\tERROR: %s\n", err);
  exit(1);
}
#endif /* ERROR_FATAL_H */

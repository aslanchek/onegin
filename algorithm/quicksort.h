#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>

#define RED   "\x1b[31m" // red
#define GRE   "\x1b[32m" // green
#define YEL   "\x1b[33m" // yellow
#define BLU   "\x1b[34m" // blue
#define MAG   "\x1b[35m" // magenta
#define CYA   "\x1b[36m" // cyan
#define RST   "\x1b[0m"  // reset color

size_t _randrange(size_t left, size_t right);

#ifdef VERBOSE
void dump_mem(int *left, int *right);
void dump_array(int *arr, size_t nmemb);
#endif

void _swap(void *a, void *b, size_t size);
int *_hoare_partition(int *left, int *right);
void _quicksort(int *left, int *right);


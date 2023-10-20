#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/stat.h>

//  сравнивает две строчки, аргументы есть char** - указатель на строку
//  s1 - константный указатель на указатель на uint32_t
int utf8_strcmp(const void *sp1, const void *sp2) {
    const uint32_t *s1 = *(const uint32_t * const *) sp1;
    const uint32_t *s2 = *(const uint32_t * const *) sp2;

    while ( *s1 != '\n') {
      if (*s1 != *s2) {
        break;
      }

    s1++;
    s2++;
    }

    return (int) (*(const uint32_t *) s1 - *(const uint32_t *) s2);
}

size_t utf8_strlen(const uint32_t *str) {
    size_t count = 0;
    
    while (*str++ != '\n') {
        count++;
    }

    return count;
}


//  сравнивает две строчки, оканчивающиеся на \n, с конца лексикографически
int utf8_linecmp_r(const void *sp1, const void *sp2) {
    const uint32_t *s1 = *(const uint32_t * const *) sp1;
    const uint32_t *s2 = *(const uint32_t * const *) sp2;

    s1 += utf8_strlen(s1) - 1;
    s2 += utf8_strlen(s2) - 1;

    while ( *s1 != '\n' ) {
      if (*s1 != *s2) {
        break;
      }

    s1--;
    s2--;
    }

    return (int) (*(const uint32_t *) s1 - *(const uint32_t *) s2);
}

// compares only the first (at most) n utf8 characters
int utf8_strncmp(const uint32_t *s1, const uint32_t *s2, size_t n);



static long int fsize(int fildes) {
    struct stat meta;

    if (fstat(fildes, &meta) < 0) {
      return -1;
    }

    return meta.st_size;
}

static size_t getnlines(uint32_t *buffer, size_t n) {
    size_t res = 0;

    for (size_t i = 0; i < n; i++) {
      res += buffer[i] == '\n';
    }

    return res;
}


/*
 *       rawstr
 *       V
 * off + 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16
 *  _____________________________________________________
 *  | | |c |c |c |\n|a |a |a |\n|d |d |d |\n|f |f |f |\n|
 *  -----------------------------------------------------
 *       ^           ^           ^           ^
 *       |           |           |           |
 *       |  +--------+           |           |
 *       |  |  +-----------------+           |
 *       |  |  |  +--------------------------+
 *       |  |  |  |                      
 *       V  V  V  V                       
 *      ________________
 *      |00|04|08|12|00|
 *      ----------------
 *       ^
 *       lines
 *
 *              null-terminating        
 *                |   byte
 *                V
 *  _____________________________________________________
 *  | | |c |c |c |\0|a |a |a |\0|d |d |d |\0|f |f |f |\0|
 *  -----------------------------------------------------
 *
 */

// Выделяет память под массив указателей на строки.
// Заполняет массив строк (указателей на строки)
static uint32_t **preprocess(uint32_t *rawstr, size_t n) {

    uint32_t **lines = calloc(getnlines(rawstr, n), sizeof(uint32_t *));

    lines[0] = rawstr;

    for (size_t i = 0, j = 1; i < n - 1; i++) {
      if (rawstr[i] == '\n') {
        lines[j++] = rawstr + i + 1;
      }
    }

    return lines;
}

// Освобождает память выделенную под массив строк
static void postprocess(uint32_t **lines) {
    free(lines);
}

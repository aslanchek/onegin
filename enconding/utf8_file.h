#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>

typedef struct {
  int fildes;
  off_t offset;
} utf8_file_t;

/*
 * Write to utf8_file_t
 */
int utf8_write(utf8_file_t* f, const uint32_t* str, size_t count);

/**
 * Read from utf8_file_t
 *
 * EXAMPLE:
 *       leading byte             continuing bytes
 * raw -> 0b110'01010          0b10'101010 0b10'101010   next byte sequence
 *        ^     ^   ^          ^    ^    ^ ^             ^  
 *        0      \ /           1     \  /  2             3  
 *                |                   |
 * seqcount = 3   |                   |
 *                -                   |
 * leadmask = 0b1111                  -
 *                       contmask = 0b111111
 *
 * result: 0101 101010 101010
 *
 */
ssize_t utf8_read(utf8_file_t* f, uint32_t* parsed, size_t count);

/*
 * Constructor
 */
utf8_file_t* utf8_fromfd(int fd);

/*
 * Destructor
 */
int utf8_close(utf8_file_t *file);


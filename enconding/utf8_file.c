#include "utf8_file.h"
#include <stdint.h>
#include <sys/types.h>

// https://kevinboone.me/overlong.html

int utf8_write(utf8_file_t* f, const uint32_t* str, size_t count) {

    ssize_t ret = -1;

    size_t i = 0;

    for (; i < count; i++) {
        uint8_t raw[6] = { 0, 0b10000000, 0b10000000, 0b10000000, 0b10000000, 0b10000000 };
        size_t raw_len = 0;

        uint32_t codepoint = str[i];

        if (codepoint <= 0x7f) {
            // 1 октет
            raw[0] |= codepoint & 0b01111111;
            raw_len = 1;

        } else if (codepoint <= 0x7ff) {
            // 2 октета
            raw[0] = 0b11000000;

            raw_len = 2;

            for (size_t b = raw_len-1; b > 0; b--) {
                raw[b] |= codepoint & 0b00111111;
                codepoint >>= 6;
            }

            raw[0] |= codepoint & 0b00011111;

        } else if (codepoint <= 0xffff) {
            // 3 октета
            raw[0] = 0b11100000;

            raw_len = 3;

            for (size_t b = raw_len-1; b > 0; b--) {
                raw[b] |= codepoint & 0b00111111;
                codepoint >>= 6;
            }
            raw[0] |= codepoint & 0b00001111;

        } else if (codepoint <= 0x1fffff) {
            // 4 октета
            raw[0] = 0b11110000;

            raw_len = 4;

            for (size_t b = raw_len-1; b > 0; b--) {
                raw[b] |= codepoint & 0b00111111;
                codepoint >>= 6;
            }

            raw[0] |= codepoint & 0b00000111;

        } else if (codepoint <= 0x3ffffff) {
            // 5 октетов
            raw[0] = 0b11111000;
            raw_len = 5;

            for (size_t b = raw_len-1; b > 0; b--) {
                raw[b] |= codepoint & 0b00111111;
                codepoint >>= 6;
            }

            raw[0] |= codepoint & 0b00000011;
            
        } else if (codepoint <= 0x7fffffff) {
            // 6 октета
            raw[0] = 0b11111100;
            raw_len = 6;

            for (size_t b = raw_len-1; b > 0; b--) {
                raw[b] |= codepoint & 0b00111111;
                codepoint >>= 6;
            }

            raw[0] |= codepoint & 0b00000001;
            
        }

        ret = write(f->fildes, raw, raw_len);

        if (ret < 0) {
            return -1;
        }

    }

    return (int) i;
}


/*
 * Parse number of bytes in character
 *
 * _c_ is considered to be the leading byte of byte sequence
 *
 * returns numbers of bytes representing 
 *         unicode character
 */
static int _bytes_in_seq(uint8_t c);


static int _bytes_in_seq(uint8_t c) {
  if ( (c & 0b10000000) == 0 ) {
    // one-byte seq: ASCII-like
    return 1;
  } else if ( (c & 0b11100000) == 0b11000000 ) {
    // two-byte seq
    return 2;
  } else if ( (c & 0b11110000) == 0b11100000 ) {
    // three-byte seq
    return 3;
  } else if ( (c & 0b11111000) == 0b11110000 ) {
    // four-byte seq
    return 4;
  } else if ( (c & 0b11111100) == 0b11111000 ) {
    // five-byte seq
    return 5;
  } else if ( (c & 0b11111110) == 0b11111100 ) {
    // six-byte seq
    return 6;
  }

  return -1;
}

#if 0
/*
 * Set file read offset in utf8_file_t
 */
static void _set_offset(utf8_file_t* f, off_t offset);
#endif

static void _set_offset(utf8_file_t *f, off_t offset) {
    f->offset += offset;
}

#if 0
/*
 * Move file read offset to _f->offset_
 *
 * f->offset is equal to zero to new utf8 file
 *
 */
off_t _utf8_reseek(utf8_file_t* f);
#endif

static off_t _utf8_reseek(utf8_file_t* f) {
    return lseek(f->fildes, f->offset, SEEK_SET);
}

ssize_t utf8_read(utf8_file_t* f, uint32_t* parsed, size_t count) {
    _utf8_reseek(f);

    // size of res == count

    uint8_t *buffer_start = calloc(count, sizeof(uint32_t));
    uint8_t *raw_bytes    = buffer_start;

    // reads raw bytes from file
    ssize_t readret = read(f->fildes, raw_bytes, count * sizeof(uint32_t));

    // errorneous cases
    if (readret == 0) {
        #ifdef DEBUG
        fprintf(stderr, "FEOF\n");
        #endif
        return 0;
    }

    if (readret < 0) {
        #ifdef DEBUG
        fprintf(stderr, "FERR\n");
        #endif
        return -1;
    }

    // if all is ok
    // raw_len - length of non-null raw byte sequence
    size_t raw_len = (size_t) readret;

    // TODO: here we can parse BOM - byte order mask

    size_t parsed_pos = 0;
    size_t raw_pos = 0;

    // while raw bytes is not out
    //             and
    // while _count_ utf8 chars is not read
    while ( raw_pos < raw_len && parsed_pos < count ) {

        // getting number of octets in unicode char
        int ret = _bytes_in_seq(raw_bytes[raw_pos]);

        if (ret < 0) { // utf-8 parse error
            return -1;
        }

        uint8_t seqcount = (uint8_t) ret;

        /**
         * 0xxxxxxx <- leading byte of one-byte sequence
         *  ^     ^
         *  1111111  <- leadmask
         *
         * 110xxxxx <- leading byte of two-byte sequence
         *    ^   ^
         *    11111  <- leadmask
         */
        uint8_t leadmask = (uint8_t) (  (seqcount == 1) ? 0b01111111 : (1 << (7 - seqcount)) - 1  );
        uint8_t contmask = 0b00111111; //<- continuation bytes mask (if such present)

        parsed[parsed_pos] = raw_bytes[raw_pos + 0] & leadmask;

        for (size_t i = 1; i < seqcount; i++) {
            parsed[parsed_pos] <<= 6; 
            parsed[parsed_pos] |=  raw_bytes[raw_pos + i] & contmask;
        }

        #ifdef DEBUG
        fprintf(stderr, "character: %lc codepoint: 0x%x seqcount: %d\n", parsed[parsed_pos], parsed[parsed_pos], seqcount);
        #endif

        raw_pos += seqcount;
        parsed_pos++;
    }

    _set_offset(f, (off_t) raw_pos);

    free(buffer_start);
    
    return (ssize_t) parsed_pos;
}

utf8_file_t* utf8_fromfd(int fd) {
    utf8_file_t *new = calloc(1 , sizeof(utf8_file_t));

    if (new) {
        new->fildes = fd;
        new->offset = 0;

        return new;
    } else {
        return NULL;
    }
}

int utf8_close(utf8_file_t *file) {
    if (!file) {
        return -1;
    }

    int res = close(file->fildes);

    if (res >= 0) {
        file->fildes = -1;
        file->offset = 0;

        free(file);
    }

    return res;
}


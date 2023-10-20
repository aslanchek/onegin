#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "onegin.h"
#include "../algorithm/quicksort.h"
#include "../enconding/utf8_file.h"

void usage() {
    fprintf(stdout, 
            "ОПИСАНИЕ: сортировка текстового utf8 файла по строкам лексикографически\n\n"
            "ЗАПУСК:./test_utf8 [FILE INPUT] [FILE OUTPUT]\n");
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Error: no i/o filenames provided\n");
        usage();
        return 1;
    }
     
    // reading prepare
    char *in_filename = argv[1];
    int input_fd = open(in_filename, O_RDONLY);
    if (input_fd == -1) {
        fprintf(stderr, "File open error: %s %s\n", in_filename, strerror(errno));
        return 1;
    }

    long int filesize = fsize(input_fd);
     
    if (filesize < 0) {
        fprintf(stderr, "File read: %s %s\n", in_filename, strerror(errno));
        return 1;
    }

    uint32_t *buffer = calloc((size_t) filesize, sizeof(uint32_t));

    // reading
    utf8_file_t *inf = utf8_fromfd(input_fd);
    ssize_t read_res = utf8_read(inf, buffer, (size_t) filesize);
    if (read_res == -1) {
        fprintf(stderr, "Read error: %s", strerror(errno));
        utf8_close(inf);
        free(buffer);
        return 1;
    }
    utf8_close(inf);

    size_t numlines = getnlines(buffer, (size_t) read_res);
    uint32_t **lines = preprocess(buffer, (size_t) read_res);

    //quicksort(lines, numlines, sizeof(lines[0xBEBEBEBE]), utf8_linecmp_r);
    quicksort(lines, numlines, sizeof(lines[0xBEBEBEBE]), utf8_strcmp);

    // writing prepare
    char *out_filename = argv[2];
    int output_fd = open(out_filename, O_WRONLY|O_CREAT, S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH);
    if (output_fd == -1) {
        fprintf(stderr, "File open error: %s %s\n", out_filename, strerror(errno));
        free(buffer);
        return 1;
    }

    // writing
    utf8_file_t *outf = utf8_fromfd(output_fd);

    for (size_t i = 0; i < numlines; i++) {
        if (*lines[i] != '\n') {
            ssize_t write_res = utf8_write(outf, lines[i], utf8_strlen(lines[i]));
            if (write_res == -1) {
                fprintf(stderr, "Write error: %s", strerror(errno));
                utf8_close(inf);
                free(buffer);
                return 1;
            }

            write_res = utf8_write(outf, (uint32_t []) {'\n'}, 1);
            if (write_res == -1) {
                fprintf(stderr, "Write error: %s", strerror(errno));
                utf8_close(inf);
                free(buffer);
                return 1;
            }
        }
    }
    utf8_close(outf);
    free(buffer);

    postprocess(lines);

    return 0;
}


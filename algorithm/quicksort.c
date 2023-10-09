#include "quicksort.h"

// возвращает случайное число 
// в диапазоне [left, right) <- не включительно
// где left и right >= 0
size_t _randrange(size_t left, size_t right) {
    return (size_t) rand() % (right - left) + left;
}

#ifdef VERBOSE
void dump_mem(int *left, int *right) {
    size_t length = (size_t) (right - left);

    for (size_t i = 0; i < length; i++) {
        fprintf(stderr, "%2d ", left[i]);
    }
}
#endif

void dump_array(int *arr, size_t nmemb) {
    fprintf(stderr, "[ ");

    for (size_t i = 0; i < nmemb; i++) {
        fprintf(stderr, "%2d ", arr[i]);
    }

    fprintf(stderr, "]\n");
}

void _swap(void *a, void *b, size_t size) {
    unsigned char *accesible_a = a;
    unsigned char *accesible_b = b;
    unsigned char tmp = 0;

    for (size_t i = 0; i < size; i++) {
        tmp = accesible_a[i];
        accesible_a[i] = accesible_b[i];
        accesible_b[i] = tmp;
    }
}

int *_hoare_partition(int *left, int *right) {
    int *pivot = left + _randrange(0, (size_t) (right-left)); 

    #ifdef VERBOSE
        size_t length = (size_t) (right - left);

        /*
         *   ....|_|_|_|_|_|_|_|_|_|_|_|_|_....
         * left   ^             ^         ^ right
         * array  ^             pivot           
         *
         *   ....|_|_|_|_|_|_|_|_|_|_|_|_|_....
         *        0 1 2 3 4 5 6 7 8 9 
         *                      ^ pivot - left = 7
         *
         */

        fprintf(stderr, "------Partition Dump------\n");
        
        fprintf(stderr, "Part:\n");
        dump_array(left, length);

        fprintf(stderr, "pivot: %2d\n", *pivot);
    #endif

    int *ip = left;
    int *jp = right-1;

    while (ip < jp) {
        while( *ip <= *pivot && ip < pivot) {
            ip++;
        }

        while( *jp > *pivot && jp > pivot ) {
            jp--;
        }

        if (ip == pivot && jp == pivot) {
            #ifdef VERBOSE
                fprintf(stderr, "[ ");
                dump_mem(left, ip);
                fprintf(stderr, GRE "%2d " RST, *ip); // green
                dump_mem(ip + 1, right);
                fprintf(stderr, "]\n");
            #endif
            // nothing to do
            break;
        } else if (ip < pivot && jp == pivot) {
            #ifdef VERBOSE
                fprintf(stderr, "[ ");

                dump_mem(left, ip);
                fprintf(stderr, RED "%2d " RST, *ip); // red
                dump_mem(ip+1, jp);
                fprintf(stderr, RED "%2d " RST, *jp); // red
                dump_mem(jp + 1, right);

                fprintf(stderr, "]\n");
            #endif

            // action
            _swap(ip, jp, sizeof(int));
            pivot = ip;

            // result
            #ifdef VERBOSE
                fprintf(stderr, "[ ");

                dump_mem(left, ip);
                fprintf(stderr, MAG "%2d " RST, *ip); // magenta
                dump_mem(ip+1, jp);
                fprintf(stderr, MAG "%2d " RST, *jp); // magenta
                dump_mem(jp + 1, right);

                fprintf(stderr, "]\n");
            #endif

//            break;
        } else if (ip == pivot && jp > pivot) {
            #ifdef VERBOSE
                fprintf(stderr, "[ ");

                dump_mem(left, ip);
                fprintf(stderr, RED "%2d " RST, *ip); // red
                dump_mem(ip+1, jp);
                fprintf(stderr, RED "%2d " RST, *jp); // red
                dump_mem(jp + 1, right);

                fprintf(stderr, "]\n");
            #endif

            // action
            _swap(ip, jp, sizeof(int));
            pivot = jp;

            // result
            #ifdef VERBOSE
                fprintf(stderr, "[ ");

                dump_mem(left, ip);
                fprintf(stderr, MAG "%2d " RST, *ip); // magenta
                dump_mem(ip+1, jp);
                fprintf(stderr, MAG "%2d " RST, *jp); // magenta
                dump_mem(jp + 1, right);

                fprintf(stderr, "]\n");
            #endif

//            break;
        } else if (ip < pivot && jp > pivot) {
            #ifdef VERBOSE
                fprintf(stderr, "[ ");

                dump_mem(left, ip);
                fprintf(stderr, RED "%2d " RST, *ip); // red
                dump_mem(ip + 1, pivot);
                fprintf(stderr, GRE "%2d " RST, *pivot); // green
                dump_mem(pivot + 1, jp);
                fprintf(stderr, RED "%2d " RST, *jp); // red
                dump_mem(jp + 1, right);

                fprintf(stderr, "]\n");
            #endif

            // action
            _swap(ip, jp, sizeof(int));

            // result
            #ifdef VERBOSE
                fprintf(stderr, "[ ");

                dump_mem(left, ip);
                fprintf(stderr, MAG "%2d " RST, *ip); // magenta
                dump_mem(ip + 1, pivot);
                fprintf(stderr, GRE "%2d " RST, *pivot); // green
                dump_mem(pivot + 1, jp);
                fprintf(stderr, MAG "%2d " RST, *jp); // magenta
                dump_mem(jp + 1, right);

                fprintf(stderr, "]\n");
            #endif
        }
    }

    return pivot;
}

void _quicksort(int *left, int *right) {
    if (right - left <= 1)
        return;

    if (right - left == 2) {
        if ( *left > *(right-1) ) {
            _swap(left, right-1, sizeof(int));
        }
        return;
    }

    int *pivot = _hoare_partition(left, right);

    _quicksort(left, pivot);
    _quicksort(pivot + 1, right);
}


#include "quicksort.h"

// возвращает случайное число 
// в диапазоне [a, b) <- не включительно
// где a и b >= 0
size_t _randrange(size_t a, size_t b) {
    return (size_t) rand() % (b - a) + a;
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

// |_|_|_|_|_|_|
//  ^           ^
// left        right - excluding
char *_hoare_partition(char *left, char *right, size_t size, compar_f cmp) {
    char *pivot = left + size * _randrange(0, (size_t) (right-left) / size);

     #ifdef VERBOSE
        size_t length = (size_t) (right - left) / size;

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
        dump_array((int *)left, length);

        fprintf(stderr, "pivot: %2d\n", *(int *)pivot);
    #endif

    char *ip = left;
    char *jp = right - size;

    while (ip < jp) {
        while( cmp(ip, pivot) <= 0 && ip < pivot) {
            ip += size;
        }

        while( cmp(jp, pivot) > 0  && jp > pivot) {
            jp -= size;
        }

        if (ip == pivot && jp == pivot) {
            #ifdef VERBOSE
                fprintf(stderr, "[ ");
                dump_mem((int *)left, (int *)ip);
                fprintf(stderr, GRE "%2d " RST, *(int *)ip); // green
                dump_mem((int *)ip + 1, (int *)right);
                fprintf(stderr, "]\n");
            #endif
            // nothing to do
        } else if (ip < pivot && jp == pivot) {
            #ifdef VERBOSE
                fprintf(stderr, "[ ");

                dump_mem((int *)left, (int *)ip);
                fprintf(stderr, RED "%2d " RST, *(int *)ip); // red
                dump_mem((int *)ip+1, (int *)jp);
                fprintf(stderr, RED "%2d " RST, *(int *)jp); // red
                dump_mem((int *)jp + 1, (int *)right);

                fprintf(stderr, "]\n");
            #endif
            // action
            _swap(ip, jp, size);
            pivot = ip;

            // result
            #ifdef VERBOSE
                fprintf(stderr, "[ ");

                dump_mem((int *)left, (int *)ip);
                fprintf(stderr, MAG "%2d " RST, *(int *)ip); // magenta
                dump_mem((int *)ip+1, (int *)jp);
                fprintf(stderr, MAG "%2d " RST, *(int *)jp); // magenta
                dump_mem((int *)jp + 1, (int *)right);

                fprintf(stderr, "]\n");
            #endif

        } else if (ip == pivot && jp > pivot) {
            #ifdef VERBOSE
                fprintf(stderr, "[ ");

                dump_mem((int *)left, (int *)ip);
                fprintf(stderr, RED "%2d " RST, *(int *)ip); // red
                dump_mem((int *)ip+1, (int *)jp);
                fprintf(stderr, RED "%2d " RST, *(int *)jp); // red
                dump_mem((int *)jp + 1, (int *)right);

                fprintf(stderr, "]\n");
            #endif
            // action
            _swap(ip, jp, size);
            pivot = jp;

            // result
            #ifdef VERBOSE
                fprintf(stderr, "[ ");

                dump_mem((int *)left, (int *)ip);
                fprintf(stderr, MAG "%2d " RST, *(int *)ip); // magenta
                dump_mem((int *)ip+1, (int *)jp);
                fprintf(stderr, MAG "%2d " RST, *(int *)jp); // magenta
                dump_mem((int *)jp + 1, (int *)right);

                fprintf(stderr, "]\n");
            #endif

        } else if (ip < pivot && jp > pivot) {
            #ifdef VERBOSE
                fprintf(stderr, "[ ");

                dump_mem((int *)left, (int *)ip);
                fprintf(stderr, RED "%2d " RST, *(int *)ip); // red
                dump_mem((int *)ip + 1, (int *)pivot);
                fprintf(stderr, GRE "%2d " RST, *(int *)pivot); // green
                dump_mem((int *)pivot + 1, (int *)jp);
                fprintf(stderr, RED "%2d " RST, *(int *)jp); // red
                dump_mem((int *)jp + 1, (int *)right);

                fprintf(stderr, "]\n");
            #endif
            // action
            _swap(ip, jp, size);

            // result
            #ifdef VERBOSE
                fprintf(stderr, "[ ");

                dump_mem((int *)left, (int *)ip);
                fprintf(stderr, MAG "%2d " RST, *(int *)ip); // magenta
                dump_mem((int *)ip + 1, (int *)pivot);
                fprintf(stderr, GRE "%2d " RST, *(int *)pivot); // green
                dump_mem((int *)pivot + 1, (int *)jp);
                fprintf(stderr, MAG "%2d " RST, *(int *)jp); // magenta
                dump_mem((int *)jp + 1, (int *)right);

                fprintf(stderr, "]\n");
            #endif
        }
    }

    return pivot;
}


void quicksort(void *base, 
               size_t nmemb, 
               size_t size,
               compar_f cmp) {

    if (nmemb <= 1)
        return;

    char *left  = (char *) base;
    char *right = (char *) base + size * nmemb;

    if (nmemb == 2) {
        if ( cmp(left, right - size) > 0 ) {
            _swap(left, right - size, size);
        }
        return;
    }

    char *pivot = _hoare_partition(left, right, size, cmp);

    size_t lpartnmemb = (size_t)(pivot - left)/size;
    quicksort(left,         lpartnmemb, size, cmp);

    size_t rpartnmemb = (size_t)(right - (pivot + size))/size;
    quicksort(pivot + size, rpartnmemb, size, cmp);
}


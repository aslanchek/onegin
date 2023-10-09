#include "quicksort.h"
#include <math.h>
#include <time.h>

// d1 < d2 -> < 0
// d1 = d2 -> = 0
// d1 > d2 -> > 0
int cmp_int(const void *dp1, const void *dp2) {
    int d1 = *(int *)dp1;
    int d2 = *(int *)dp2;
    return d1 == d2 ? 0 : d1 < d2 ? -1 : 1;
}

#define EPSILON 0.00001
int cmp_double(const void *dp1, const void *dp2) {
    double d1 = *(double *) dp1;
    double d2 = *(double *) dp2;

    // abs(x) < E <=> x < E and x > -E
    // x > -E <=> -x < E
    if ( fabs(d1 - d2) < EPSILON ) {
        return 0;
    } else if ( d1 - d2 < 0 ) {
        return -1;
    } else {
        return 1;
    }
}

int main() {
    srand(time(NULL));
    {
    fprintf(stderr, "-------Тест 1--------------\n");
    fprintf(stderr, "-------До сортировки-------\n");

    int ARRAY[] = {10, 2, 8};

    size_t sz = 3;

    fprintf(stderr, "[ ");
    for (size_t i = 0; i < sz; i++) {
        fprintf(stderr, "%d ", ARRAY[i]);
    }
    fprintf(stderr, "]\n");

    quicksort(ARRAY, sz, sizeof(int), cmp_int);

    fprintf(stderr, "----После сортировки-------\n");

    fprintf(stderr, "[ ");
    for (size_t i = 0; i < sz; i++) {
        fprintf(stderr, "%d ", ARRAY[i]);
    }
    fprintf(stderr, "]\n");


    }

    printf("Нажмите Enter чтобы продолжить");
    while( getchar() != '\n' ) {};

    {
    fprintf(stderr, "-------Тест 2--------------\n");
    fprintf(stderr, "-------До сортировки-------\n");

    int ARRAY[] = {10, 2, 8, 1, 5, 8, 7, 3, 9, 4, 5, 2};

    size_t sz = 12;

    fprintf(stderr, "[ ");
    for (size_t i = 0; i < sz; i++) {
        fprintf(stderr, "%d ", ARRAY[i]);
    }
    fprintf(stderr, "]\n");

    quicksort(ARRAY, sz, sizeof(int), cmp_int);

    fprintf(stderr, "----После сортировки-------\n");

    fprintf(stderr, "[ ");
    for (size_t i = 0; i < sz; i++) {
        fprintf(stderr, "%d ", ARRAY[i]);
    }
    fprintf(stderr, "]\n");

    }

    printf("Нажмите Enter чтобы продолжить");
    while( getchar() != '\n' ) {};

    {
    fprintf(stderr, "-------Тест 3--------------\n");
    fprintf(stderr, "-------До сортировки-------\n");

    int ARRAY[] = {5, 1, 1, 5, 1, 1, 5};

    size_t sz = 7;

    fprintf(stderr, "[ ");
    for (size_t i = 0; i < sz; i++) {
        fprintf(stderr, "%d ", ARRAY[i]);
    }
    fprintf(stderr, "]\n");

    quicksort(ARRAY, sz, sizeof(int), cmp_int);

    fprintf(stderr, "----После сортировки-------\n");

    fprintf(stderr, "[ ");
    for (size_t i = 0; i < sz; i++) {
        fprintf(stderr, "%d ", ARRAY[i]);
    }
    fprintf(stderr, "]\n");

    }

    printf("Нажмите Enter чтобы продолжить");
    while( getchar() != '\n' ) {};

    {
    fprintf(stderr, "-------Тест 4--------------\n");
    fprintf(stderr, "-------До сортировки-------\n");

    double ARRAY[] = {5.0, 1.2, 1.8, 5.7, 1.2, 1.0, 5.7};

    size_t sz = 7;

    fprintf(stderr, "[ ");
    for (size_t i = 0; i < sz; i++) {
        fprintf(stderr, "%.1lf ", ARRAY[i]);
    }
    fprintf(stderr, "]\n");

    quicksort(ARRAY, sz, sizeof(double), cmp_double);

    fprintf(stderr, "----После сортировки-------\n");

    fprintf(stderr, "[ ");
    for (size_t i = 0; i < sz; i++) {
        fprintf(stderr, "%.1lf ", ARRAY[i]);
    }
    fprintf(stderr, "]\n");

    }

    return 0;
}


#include "quicksort.h"
#include <time.h>

// d1 < d2 -> < 0
// d1 = d2 -> = 0
// d1 > d2 -> > 0
int compar_int(const void *dp1, const void *dp2) {
    int d1 = *(int *)dp1;
    int d2 = *(int *)dp2;
    return d1 == d2 ? 0 : d1 < d2 ? -1 : 1;
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

    quicksort(ARRAY, sz, sizeof(int), compar_int);

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

    quicksort(ARRAY, sz, sizeof(int), compar_int);

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

    quicksort(ARRAY, sz, sizeof(int), compar_int);

    fprintf(stderr, "----После сортировки-------\n");

    fprintf(stderr, "[ ");
    for (size_t i = 0; i < sz; i++) {
        fprintf(stderr, "%d ", ARRAY[i]);
    }
    fprintf(stderr, "]\n");

    }

    return 0;
}


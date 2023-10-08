#include "quicksort.h"

int main() {
    #ifdef VERBOSE
    fprintf(stderr, "-------Цвета означают------\n");
    fprintf(stderr, GRE "■" RST " - выбранный pivot"  "\n");
    fprintf(stderr, RED "■" RST " - те, что меняются на данном шаге" "\n");
    fprintf(stderr, MAG "■" RST " - индикация перемещения элементов"  "\n");
    #endif

    {
    #ifdef VERBOSE
    fprintf(stderr, "-------Тест 1--------------\n");
    fprintf(stderr, "-------До сортировки-------\n");
    #endif

    int ARRAY[] = {10, 2, 8, 1, 5, 8, 7, 3, 9, 4, 5, 2};

    dump_array(ARRAY, 12);

    _quicksort(ARRAY, ARRAY + 12);

    #ifdef VERBOSE
    fprintf(stderr, "----После сортировки-------\n");
    #endif

    dump_array(ARRAY, 12);
    }
    printf("Нажмите Enter чтобы продолжить\n");
    while( getchar() != '\n' ) {};
    // #################################################
    {
    #ifdef VERBOSE
    fprintf(stderr, "-------Тест 2--------------\n");
    fprintf(stderr, "-------До сортировки-------\n");
    #endif

    int ARRAY[] = {3, 2, 1};

    dump_array(ARRAY, 3);

    _quicksort(ARRAY, ARRAY + 3);

    #ifdef VERBOSE
    fprintf(stderr, "----После сортировки-------\n");
    #endif

    dump_array(ARRAY, 3);
    }
    printf("Нажмите Enter чтобы продолжить\n");
    while( getchar() != '\n' ) {};
    // #################################################
    {
    #ifdef VERBOSE
    fprintf(stderr, "-------Тест 3--------------\n");
    fprintf(stderr, "-------До сортировки-------\n");
    #endif

    int ARRAY[] = {5, -1, 5, 0, 5};

    dump_array(ARRAY, 5);

    _quicksort(ARRAY, ARRAY + 5);

    #ifdef VERBOSE
    fprintf(stderr, "----После сортировки-------\n");
    #endif

    dump_array(ARRAY, 5);
    }

    return 0;
}


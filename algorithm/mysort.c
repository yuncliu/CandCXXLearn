#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAXMUM 1000
#define LENGTH 1000

int bubble_sort(int array[], int size);

int quick_sort(int array[], int size);
int quick_sort_r(int array[], int start, int end);
int quick_sort_partion(int array[], int start, int end);

void produce_array(int arrary[], int size);
void print_array(int array[], int size);

typedef int (*sort_function)(int[], int);
clock_t get_clock_time(sort_function func, int array[], int size);


int main(int argc, char* argv[]) {
    int array1[LENGTH] = {0};
    int array2[LENGTH] = {0};
    produce_array(array1, LENGTH);
    produce_array(array2, LENGTH);

    printf("Bubble time:"); 
    get_clock_time(bubble_sort, array1, LENGTH);
    printf("Quick time :") ;
    get_clock_time(quick_sort, array2, LENGTH);
    return 0;
}

clock_t get_clock_time(sort_function func, int array[], int size) {
    clock_t start = clock();
    func(array, size);
    clock_t end = clock();
    printf("cpu time [%ld]\n", end - start);
    return end - start;
}


void produce_array(int array[], int size) {
    static int is_seed_set = 0;
    if (0 == is_seed_set ) {
       srand(time(NULL)); 
       is_seed_set = 1;
    }
    int i = 0;
    for (i = 0; i < size; ++i) {
        array[i] = rand() % MAXMUM;
    }
}

void print_array(int array[], int size) {
    int i = 0;
    for (i = 0; i < size; ++i) {
        printf("%d ", array[i]);
    }
    printf("\n");
}

int bubble_sort(int array[], int size) {
    int i,j;
    for (j = size - 1; j > 0 ; --j) {
        for (i = 0; i < j; ++i) {
            if (array[i] > array[i+1]) {
                array[i] = array[i] ^ array[i+1];
                array[i+1] = array[i] ^ array[i+1];
                array[i] = array[i] ^ array[i+1];
            }
        }
    }
    return 0;
}

int quick_sort(int array[], int size) {
    quick_sort_r(array, 0, size);
    return 0;
}

int quick_sort_r(int array[], int start, int end) {
    if (start  < end) {
        int j  = quick_sort_partion(array, start, end);
        quick_sort_r(array, start, j - 1);
        quick_sort_r(array, j + 1, end);
    }
    return 0;
}

int quick_sort_partion(int array[], int start, int end) {
    int pivot, i, j, t;
    pivot = array[start];
    i = start; 
    j = end + 1;

    while( 1)
    {
        do ++i; while( array[i] <= pivot && i <= end );
        do --j; while( array[j] > pivot );
        if( i >= j ) break;
        t = array[i]; array[i] = array[j]; array[j] = t;
    }
    t = array[start]; array[start] = array[j]; array[j] = t;
    return j;
}

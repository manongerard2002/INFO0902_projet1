/* ========================================================================= *
 * QuickSort
 * Implementation of the QuickSort algorithm.
 * ========================================================================= */

#include "Sort.h"
#include "Array.h"


/* ------------------------------------------------------------------------- *
 * Sort an array of integers using quicksort.
 *
 * PARAMETERS
 * array        The array to sort
 * p            The index of the first element to sort
 * r            The index of the last element to sort
 * ------------------------------------------------------------------------- */
static void quickSort(int* array, int p, int r);

/* ------------------------------------------------------------------------- *
 * Gives the index of the partition of quicksort.
 *
 * PARAMETERS
 * array        The array
 * p            The index of the first element to partition
 * r            The index of the pivot
 * 
 * RETURN
 * q            The index of the partition
 * ------------------------------------------------------------------------- */
static int partition(int* array, int p, int r);

/* ------------------------------------------------------------------------- *
 * Swap the elements pointed by a and b.
 *
 * PARAMETERS
 * a            The pointer to the first int element
 * b            The pointer to the second int element
 * ------------------------------------------------------------------------- */
static void swap(int* a, int* b);


void sort(int* array, size_t length) {
    quickSort(array, 0, length - 1);
    return;
}

static void quickSort(int* array, int p, int r) {
    if (p < r) {
        int q = partition(array, p, r);
        quickSort(array, p, q - 1);
        quickSort(array, q + 1, r);
    }
}

static int partition(int* array, int p, int r) {
    int x = array[r];
    int i = p - 1;
    for (int j = p; j <= r - 1; j++) {
        if (intCmp(array[j], x)<=0) {
        //if(array[j] <= x) {
            i = i + 1;
            swap(array + i, array + j);
        }
    }
    swap(array + i + 1, array + r);

    return i + 1;
}

static void swap(int* a, int* b) {
    int t = *a;
    *a = *b;
    *b = t;
}

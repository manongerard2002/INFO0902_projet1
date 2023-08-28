/* ========================================================================= *
 * HeapSort
 * Implementation of the HeapSort algorithm.
 * ========================================================================= */

#include "Sort.h"
#include "Array.h"

/* ------------------------------------------------------------------------- *
 * Create a heap from an array of integers.
 *
 * PARAMETERS
 * array        The array of integers to transform into a heap
 * length       Number of elements in the array
 * ------------------------------------------------------------------------- */
static void buildMaxHeap(int* array, size_t length);

/* ------------------------------------------------------------------------- *
 * Assuming that the left subtree of node i is a heap
 * and assuming that the right subtree of node i is a heap
 * Rearrange the heap to maintain the order property of the heap.
 *
 * PARAMETERS
 * array        The array of integers
 * heapSize     Number of elements in the heap
 * ------------------------------------------------------------------------- */
static void maxHeapify(int* array, size_t i, size_t heapSize);

/* ------------------------------------------------------------------------- *
 * Gives the left child node.
 * 
 * PARAMETERS
 * i            The parent node
 * 
 * RETURN
 * left         2i + 1 the left child node
 * ------------------------------------------------------------------------- */
static size_t left(size_t i);

/* ------------------------------------------------------------------------- *
 * Gives the right child node.
 * 
 * PARAMETERS
 * i            The parent node
 * 
 * RETURN
 * right        2i + 2 the right child node
 * ------------------------------------------------------------------------- */
static size_t right(size_t i);

/* ------------------------------------------------------------------------- *
 * Swap the elements pointed by a and b.
 *
 * PARAMETERS
 * a            The pointer to the first int element
 * b            The pointer to the second int element
 * ------------------------------------------------------------------------- */
static void swap(int* a, int* b);

void sort(int* array, size_t length)
{
    buildMaxHeap(array, length);
    size_t heapSize = length;
    for (size_t i = length - 1; i > 0; i--) {
        swap(array + i, array);
        heapSize = heapSize - 1;
        maxHeapify(array, 0, heapSize);
    }
    return;
}

static void buildMaxHeap(int* array, size_t length) {
    for (int i = length/2 - 1; i >= 0; i--)
        maxHeapify(array, i, length);
}

static void maxHeapify(int* array, size_t i, size_t heapSize) {
    size_t l = left(i);
    size_t r = right(i);
    size_t largest;
    if (l < heapSize && (intCmp(array[l], array[i])>0))
        largest = l;
    else
        largest = i;
    if (r < heapSize && (intCmp(array[r], array[largest])>0))
        largest = r;
    if (largest != i) {
        swap(array + i, array + largest);
        maxHeapify(array, largest, heapSize);
    }
}

static size_t left(size_t i) {
    return 2*i + 1;
}

static size_t right(size_t i) {
    return 2*i + 2;
}

static void swap(int* a, int* b) {
    int t = *a;
    *a = *b;
    *b = t;
}

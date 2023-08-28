/* ========================================================================= *
 * AdaptiveMergeSort
 * Implementation of the Adaptive Merge Sort algorithm.
 * ========================================================================= */

#include "Sort.h"
#include "Array.h"
#include <stdlib.h>

// Note: We took the implement of the stack from the course "complement d'informatique"
// A structure representing a stack (LIFO data structure).
typedef struct Stack_t Stack;

/* ------------------------------------------------------------------------- *
 * Create an empty stack.
 *
 * PARAMETERS
 * s			The new stack
 * ------------------------------------------------------------------------- */
static Stack *stackCreate(void);

/* ------------------------------------------------------------------------- *
 * Deallocate the memory occupied by a stack.
 * NB: the memory associated to the data pushed on the stack is not freed.
 *
 * PARAMETERS
 * s			The stack
 * ------------------------------------------------------------------------- */
static void stackFree(Stack *s);

/* ------------------------------------------------------------------------- *
 * Push a new element on a stack.
 *
 * PARAMETERS
 * s			The stack
 * data			The data to push on the stack.
 * ------------------------------------------------------------------------- */
static void stackPush(Stack *s, void *data);

/* ------------------------------------------------------------------------- *
 * Pop the data that is at the top of a stack and return it.
 *
 * PARAMETERS
 * s			The stack
 *
 * RETURNS
 * data			A void pointer to the data.
 * ------------------------------------------------------------------------- */
static void *stackPop(Stack *s);

/* ------------------------------------------------------------------------- *
 * Return the size of a stack.
 *
 * PARAMETERS
 * s			The stack
 *
 * RETURNS
 * size			The size of the stack.
 * ------------------------------------------------------------------------- */
static int stackSize(Stack *s);

/* ------------------------------------------------------------------------- *
 * Find an increasing run in the array starting at the start position. See
 * the statement for a description of how the function should work.
 *
 * PARAMETERS
 * array        The array
 * start        The position in array (<=last) at which to start finding a run
 * last         The last position in the array (=length of the array-1)
 * minSize      The mininum size of a run
 *
 * RETURN
 * end          The end position of the run (A[start..end] should be sorted in
 *              increasing order, end must be <= last and end-start+1 must be
 *              >= min(minSize,last-start+1)).
 * ------------------------------------------------------------------------- */
int findRun(int* array, size_t start, size_t last, size_t minSize);

/* ------------------------------------------------------------------------- *
 * If necessary elongates the run to obtain a size of minSize.
 *
 * PARAMETERS
 * array        The array
 * i			The index until which the array is sorted starting from start
 * start        The position in array (<=last) at which to start finding a run
 * last         The last position in the array (=length of the array-1)
 * minSize      The mininum size of a run
 *
 * RETURN
 * end          The end position of the run (A[start..end] should be sorted in
 *              increasing order, end must be <= last and end-start+1 must be
 *              >= min(minSize,last-start+1)).
 * ------------------------------------------------------------------------- */
static int elongation(int* array, size_t i, size_t start, size_t last, size_t minSize);

/* ------------------------------------------------------------------------- *
 * Sort a portion of an array of integers using an adaptation of insertion sort
 * which takes into account that the first i element from start are sorted.
 *
 * PARAMETERS
 * array        The array to sort
 * start        the index of the first element
 * i            the index of the last element sorted
 * ending       the index of the last element to sort
 * ------------------------------------------------------------------------- */
static void InsertSort(int* array, size_t start, size_t i, size_t ending);

/* ------------------------------------------------------------------------- *
 * Swap the elements pointed by a and b.
 *
 * PARAMETERS
 * a            The pointer to the first int element
 * b            The pointer to the second int element
 * ------------------------------------------------------------------------- */
static void swap(int* a, int* b);

/* ------------------------------------------------------------------------- *
 * Merges two subarrays of tab[].
 * First subarray is tab[lo..mid-1]
 * Second subarray is tab[mid..hi]
 *
 * PARAMETERS
 * array        The array to merge
 * lo			The index of the first element in the first subarray
 * mid			The index of the first element in the second subarray
 * hi			The index of the last element in the second array
 * aux			The auxiliary array used for merge
 * ------------------------------------------------------------------------- */
static void merge(int tab[], int lo, int mid, int hi, int aux[]);

/* ------------------------------------------------------------------------- *
 * Merges element from the top of the stack respecting the statement.
 *
 * PARAMETERS
 * array        The array to merge
 * stack		The stack
 * aux			The auxiliary array used for merge
 * ------------------------------------------------------------------------- */
static void mergeStack(int array[], Stack *stack, int aux[]);

/* ------------------------------------------------------------------------- *
 * Merge the two subarrays at the top of the stack by replacing them with the
 * subarray resulting from their merge.
 *
 * PARAMETERS
 * array        The array to sort
 * stack		The stack
 * aux			The auxiliary array used for merge
 * ------------------------------------------------------------------------- */
static void finalMergeStack(int array[], Stack *stack, int aux[]);

/* ------------------------------------------------------------------------- *
 * Merges element from the top of the stack of size two respecting the statement.
 *
 * PARAMETERS
 * array        The array to merge
 * stack		The stack
 * aux			The auxiliary array used for merge
 * ------------------------------------------------------------------------- */
static void mergeStackSize2(int array[], Stack *stack, int aux[]);

/* ------------------------------------------------------------------------- *
 * Merges element from the top of the stack of size of at least three
 * respecting the statement.
 *
 * PARAMETERS
 * array        The array to merge
 * stack		The stack
 * aux			The auxiliary array used for merge
 * ------------------------------------------------------------------------- */
static void mergeStackSize3(int array[], Stack *stack, int aux[]);


typedef struct Node_t {
	void *data;
	struct Node_t *next;
} Node;

struct Stack_t{
	Node *top;
	int size;
};

static Stack* stackCreate() {
	Stack *s = malloc(sizeof(Stack));
	if (!s)
		exit(EXIT_FAILURE);
	s -> top = NULL;
	s -> size = 0;
	return s;
}

static void stackFree(Stack *s) {
	Node *n = s -> top;
	while (n) {
		Node *nNext = n -> next;
		free(n);
		n = nNext;
	}
	free(s);
}

static void stackPush(Stack *s, void *data) {
	Node *n = malloc(sizeof(Node));
	if (!n)
		exit(EXIT_FAILURE);
	n -> data = data;
	n -> next = s -> top;
	s -> top = n;
	s -> size++;
}

static void *stackPop(Stack *s) {
	if (!(s -> top))
		exit(EXIT_FAILURE);
	Node *n = s -> top;
	void *data = n -> data;
	s -> top = n -> next;
	s -> size--;
	free(n);
	return data;
}

static int stackSize(Stack *s) {
	return s -> size;
}

static void mergeStack(int array[], Stack *stack, int aux[]) {
	if(stackSize(stack) == 1)
		return;

	if(stackSize(stack) == 2){
		mergeStackSize2(array, stack, aux);
		return;
	}
	
	if(stackSize(stack) >= 3) 
		mergeStackSize3(array, stack, aux);
}

static void mergeStackSize2(int array[], Stack *stack, int aux[]) {
	int* dataB = stackPop(stack);
	int* dataA = stackPop(stack);
	int lengthB = dataB[1] - dataB[0] + 1;
	int lengthA = dataA[1] - dataA[0] + 1;
		
	if(lengthA <= lengthB) {
		int lo = dataA[0];
		int mid = dataB[0];
		int hi = dataB[1];
		merge(array, lo, mid, hi, aux);
		dataA[1] = dataB[1];
		stackPush(stack, dataA);
	}
	else {
		stackPush(stack, dataA);
		stackPush(stack, dataB);
	}
}

static void mergeStackSize3(int array[], Stack *stack, int aux[]) {
	int* dataC = stackPop(stack);
	int* dataB = stackPop(stack);
	int* dataA = stackPop(stack);
	int lengthC = dataC[1] - dataC[0] + 1;
	int lengthB = dataB[1] - dataB[0] + 1;
	int lengthA = dataA[1] - dataA[0] + 1;
	
	if(lengthA <= lengthB + lengthC) {
		if(lengthA <= lengthC){
			int lo = dataA[0];
			int mid = dataB[0];
			int hi = dataB[1];
			merge(array, lo, mid, hi, aux);
			dataB[0] = dataA[0];
			stackPush(stack, dataB);
			stackPush(stack, dataC);
			mergeStack(array, stack, aux);
		}
		else {
			int lo = dataB[0];
			int mid = dataC[0];
			int hi = dataC[1];
			merge(array, lo, mid, hi, aux);
			dataB[1] = dataC[1];
			stackPush(stack, dataA);
			stackPush(stack, dataB);
			mergeStack(array, stack, aux);
		}
	}
	
	else {
		if(lengthB <= lengthC) {
			int lo = dataB[0];
			int mid = dataC[0];
			int hi = dataC[1];
			merge(array, lo, mid, hi, aux);
			dataB[1] = dataC[1];
			stackPush(stack, dataA);
			stackPush(stack, dataB);
			mergeStack(array, stack, aux);
		}
		else {
			stackPush(stack, dataA);
			stackPush(stack, dataB);
			stackPush(stack, dataC);
		}
	}
}

static void finalMergeStack(int array[], Stack *stack, int aux[]) {
	if(stackSize(stack) == 1)
		return;
	int* dataC = stackPop(stack);
	int* dataB = stackPop(stack);
	int lo = dataB[0];
	int hi = dataC[1];
	int mid = dataC[0];
	merge(array, lo, mid, hi, aux);
	dataB[1] = dataC[1];
	stackPush(stack, dataB);
	return;
}

void sort(int *array, size_t length) {
	
	if(!array || length == 0)
		return;
	
	int aux[length];
	int minSize = 32;
    int end = -1;
	Stack *stack = stackCreate();
	int endCounter = 0;
	int maxLenght = ((length + minSize - 1) / minSize)*2;
	int tabIndex[maxLenght];
	
    while (end < (int) length - 1) {
		tabIndex[endCounter*2] = end + 1;
		end = findRun(array, end + 1, length - 1, minSize);
		tabIndex[endCounter*2+1] = end;
		stackPush(stack, &tabIndex[endCounter*2]);
		endCounter++;
		mergeStack(array, stack, aux);
	}
	
	while (stackSize(stack) > 1)
		finalMergeStack(array, stack, aux);

	stackFree(stack);
	return;
}

int findRun(int* array, size_t start, size_t last, size_t minSize) {
	size_t i = start;
	
	// 1st case : "array sorted in reverse order"
    if(intCmp(array[start],array[start + 1]) > 0) {
        while (i < last && intCmp(array[i], array[i + 1])>=0)
            i++;
        for (size_t j = 0; j < (i - start + 1)/2; j++) {
            swap(array + start + j, array + i - j);
		}

		return elongation(array, i, start, last, minSize);
	}
	
	// 2nd case : "array sorted in the right order"
    else {
		while (i < last && intCmp(array[i], array[i + 1])<=0)
			i++;
		
		return elongation(array, i, start, last, minSize);
	}
}

static int elongation(int* array, size_t i, size_t start, size_t last, size_t minSize) {
	if (i - start + 1 < minSize) {
		size_t ending = 0;
		if(start + minSize <= last) {
			ending = start + minSize - 1;
			InsertSort(array, start, i, ending);
		}
		else {
			ending = last;
			InsertSort(array, start, i, ending);
		}
		return (int) ending;
	}
	return i;	
}

static void InsertSort(int* array, size_t start, size_t i, size_t ending) {
    size_t j;
    int tmp;
	
    // Insert A[i+1 .. ending] into the sorted sequence A[start .. i]
    for (size_t k = i + 1; k <= ending; k++) {
        tmp = array[k];
        j = k;
		// Place tmp at the right position
        while(j > start && intCmp(array[j-1],tmp)>0) {
			array[j] = array[j-1];
			j--;
        }
        array[j] = tmp;
    }
}

static void swap(int* a, int* b) {
    int t = *a;
    *a = *b;
    *b = t;
}

static void merge(int tab[], int lo, int mid, int hi, int aux[])
{
    int i = lo, j = mid;

    for (int k = lo; k <= hi; k++)
        if (i == mid)
            aux[k] = tab[j++];
        else if (j == hi + 1)
            aux[k] = tab[i++];
        else if (intCmp(tab[i], tab[j]) < 0)
            aux[k] = tab[i++];
        else
            aux[k] = tab[j++];

    for (int k = lo; k <= hi; k++) {
        tab[k] = aux[k];
	}
}

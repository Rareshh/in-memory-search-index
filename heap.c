// SPINU Rares - 311CA

#include "heap.h"

//Helper function for swapping two elements in the heap
void swap(void *a, void *b, int size)
{
    void *temp = malloc(size); //FREE
    if (!temp) {
        return;
    }
    memcpy(temp, a, size);
    memcpy(a, b, size);
    memcpy(b, temp, size);
    free(temp);
}

//Function for creating a new heap
heap_t *heap_create(int (*cmp_f)(const void *, const void *), int data_size)
{
    heap_t *heap = malloc(sizeof(heap_t)); //FREE
    if (!heap) {
        return NULL;
    }

    heap->capacity = 5;
    heap->size = 0;
    heap->data_size = data_size;
    heap->cmp = cmp_f;

    heap->arr = malloc(heap->capacity * data_size); //FREE
    if (!heap->arr) {
        free(heap);
        return NULL;
    }

    return heap;
}

//Function for moving an element up in the heap to maintain the property
void sift_up(heap_t *heap, int pos)
{
    if (pos <= 0) {
        return;
    }

    int parent = (pos - 1) / 2;
    char *arr = (char *)heap->arr;

    void *child_ptr = arr + pos * heap->data_size;
    void *parent_ptr = arr + parent * heap->data_size;

    //If the child is greater than the parent according to the compare function
    if (heap->cmp(child_ptr, parent_ptr) > 0) {
        swap(child_ptr, parent_ptr, heap->data_size);
        sift_up(heap, parent);
    }
}

//Function for inserting a new element
void heap_insert(heap_t *heap, void *data)
{
    if (!heap) {
        return;
    }

    // Reallocating if the array is full
    if (heap->size == heap->capacity) {
        heap->capacity *= 2;
        heap->arr = realloc(heap->arr, heap->capacity * heap->data_size);
    }

    char *arr = (char *)heap->arr;
    memcpy(arr + heap->size * heap->data_size, data, heap->data_size);
    
    sift_up(heap, heap->size);
    heap->size++;
}

//Function for getting the top element
void *heap_top(heap_t *heap)
{
    if (!heap || heap->size == 0) {
        return NULL;
    }
    return heap->arr;
}

//Function for moving an element down in the heap to maintain the property
void sift_down(heap_t *heap, int pos)
{
    int left = 2 * pos + 1;
    int right = 2 * pos + 2;
    int biggest = pos;
    char *arr = (char *)heap->arr;

    // Checking if the left child is bigger
    if (left < heap->size) {
        if (heap->cmp(arr + left * heap->data_size, arr + biggest * heap->data_size) > 0) {
            biggest = left;
        }
    }

    // Checking if the right child is bigger
    if (right < heap->size) {
        if (heap->cmp(arr + right * heap->data_size, arr + biggest * heap->data_size) > 0) {
            biggest = right;
        }
    }

    if (biggest != pos) {
        swap(arr + pos * heap->data_size, arr + biggest * heap->data_size, heap->data_size);
        sift_down(heap, biggest);
    }
}

// Function for removing the top element
void heap_pop(heap_t *heap)
{
    if (!heap || heap->size == 0) {
        return;
    }

    char *arr = (char *)heap->arr;
    
    // Moving the last element to the root position
    memcpy(arr, arr + (heap->size - 1) * heap->data_size, heap->data_size);
    heap->size--;

    if (heap->size > 0) {
        sift_down(heap, 0);
    }
}

// Function for freeing the heap
void heap_free(heap_t *heap)
{
    if (!heap) {
        return;
    }
    free(heap->arr);
    free(heap);
}

// Comparator function used for sorting the files in TOPK
int compare_files(const void *a, const void *b)
{
    struct file *f1 = *(struct file **)a;
    struct file *f2 = *(struct file **)b;

    // Highest score goes first
    if (f1->score != f2->score) {
        return f1->score - f2->score;
    }

    // If scores are equal, lowest ID alphabetically goes first
    return strcmp(f2->id, f1->id);
}

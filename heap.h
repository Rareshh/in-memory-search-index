// SPINU Rares - 311CA

#ifndef heap_h
#define heap_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"

typedef struct {
    void *arr;
    int size;
    int capacity;
    int data_size;
    int (*cmp)(const void *key1, const void *key2);
} heap_t;

heap_t *heap_create(int (*cmp_f)(const void *, const void *), int data_size);
void heap_insert(heap_t *heap, void *data);
void *heap_top(heap_t *heap);
void heap_pop(heap_t *heap);
void heap_free(heap_t *heap);

// Comparator function for the TOPK command
int compare_files(const void *a, const void *b);

#endif

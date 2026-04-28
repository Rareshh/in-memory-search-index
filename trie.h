// SPINU Rares - 311CA

#ifndef trie_h
#define trie_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"

#define ALPHABET_SIZE 26
#define ALPHABET "abcdefghijklmnopqrstuvwxyz"

typedef struct trie_node_t trie_node_t;
struct trie_node_t {
    void *value;
    int end_of_word;
    trie_node_t **children;
    int n_children;
};

typedef struct trie_t trie_t;
struct trie_t {
    trie_node_t *root;
    
    int size;

    int data_size;

    int alphabet_size;
    char *alphabet;

    void (*free_value_cb)(void*);

    int nNodes;
};

trie_node_t *trie_create_node(trie_t *trie);
trie_t *trie_create(int data_size, int alphabet_size, char *alphabet, void (*free_value_cb)(void*));
void trie_insert(trie_t *trie, char *key, void *value);
void *trie_search(trie_t *trie, char *key);
void trie_remove(trie_t *trie, char *key);
void trie_free(trie_t **pTrie);

#endif

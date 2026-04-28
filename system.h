// SPINU Rares - 311CA

#ifndef system_h
#define system_h

#include "list.h"
#include "trie.h"

//Using a struct system for managing the Files_List and the Trie
struct system {
    struct dl_list *list;
    struct trie_t *trie;
};

struct system *init_system();
void free_system(struct system *sys);

#endif
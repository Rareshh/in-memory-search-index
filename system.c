// SPINU Rares - 311CA

#include "system.h"

void free_trie(void *data) {
	struct dl_list *list = (struct dl_list *)data;
	free_list(&list);
}

struct system *init_system()
{
	struct system *sys = malloc(sizeof(struct system));
	if (!sys) {
		return NULL;
	}

	sys->list = create_list(sizeof(struct file));
	sys->trie = trie_create(sizeof(struct file *), ALPHABET_SIZE,
							ALPHABET, free_trie);

	return sys;
}

void free_system(struct system *sys)
{
    if (!sys) {
		return;
	}

    struct list_node *p = sys->list->head->next;
    for (int i = 0; i < sys->list->size; i++) {
        struct file *f = (struct file *)p->data;
        free(f->id);
        free_list(&f->keywords);
        p = p->next;
    }
    free_list(&sys->list);

    trie_free(&sys->trie);
    
    free(sys);
}
// SPINU Rares - 311CA

#include "trie.h"

//Function for creating a new trie node
trie_node_t *trie_create_node(trie_t *trie)
{
	trie_node_t *node = malloc(sizeof(trie_node_t));
	if (!node) {
		return NULL;
	}

	node->value = NULL;
	node->end_of_word = 0;
	node->n_children = 0;

	node->children = malloc(trie->alphabet_size * sizeof(trie_node_t *));
	if (!node->children) {
		free(node);
		return NULL;
	}

	for (int i = 0; i < trie->alphabet_size; i++) {
		node->children[i] = NULL;
	}

	trie->nNodes++;
	return node;
}

//Function for creating a trie
trie_t *trie_create(int data_size, int alphabet_size, char *alphabet, void (*free_value_cb)(void*))
{
	trie_t *trie = malloc(sizeof(trie_t));
	if (!trie) {
		return NULL;
	}

	trie->size = 0;
	trie->data_size = data_size;
	trie->alphabet_size = alphabet_size;
	trie->nNodes = 0;
	trie->free_value_cb = free_value_cb;
	trie->alphabet = strdup(alphabet);
	if (!trie->alphabet) {
		return NULL;
	}

	trie->root = trie_create_node(trie);

	return trie;
}

//Helper function for inserting a node
void trie_insert_helper(trie_t *trie, trie_node_t *curr, char *key, void *value) {
	if (strlen(key) == 0) {
		if (curr->end_of_word == 0) {
			curr->value = create_list(sizeof(struct file *));
			curr->end_of_word = 1;
			trie->size++;
		}

		struct dl_list *list = (struct dl_list *)curr->value;
    	list_add(list, list->size, &value);

		return;
	}

	trie_node_t *nextNode = curr->children[key[0] - 'a'];

	if (nextNode == NULL) {
		nextNode = trie_create_node(trie);
		curr->children[key[0] - 'a'] = nextNode;
		curr->n_children++;
	}

	trie_insert_helper(trie, nextNode, key + 1, value);
}

//Function for inserting a new node
void trie_insert(trie_t *trie, char *key, void *value) {
    if (!trie || !key) {
		return;
	}

    trie_insert_helper(trie, trie->root, key, value);
}

//Helper function for searching a node
void *trie_search_helper(trie_t *trie, trie_node_t *curr, char *key)
{
	if (key[0] == '\0') {
		if (curr->end_of_word == 1) {
			return curr->value;
		}
		return NULL;
	}

	trie_node_t *nextNode = curr->children[key[0] - 'a'];

	if (!nextNode) {
		return NULL;
	}

	return trie_search_helper(trie, nextNode, key + 1);
}

//
void *trie_search(trie_t *trie, char *key)
{
	if (!key || !trie) {
		return NULL;
	}

	return trie_search_helper(trie, trie->root, key);
}

//
int trie_remove_helper(trie_t *trie, trie_node_t *curr, char *key) {
	if (strlen(key) == 0) {
		if (curr->end_of_word == 1) {
			curr->end_of_word = 0;
			trie->size--;

			trie->free_value_cb(curr->value);
			curr->value = NULL;

			return (curr->n_children == 0);
		}
		return 0;
	}

	trie_node_t *nextNode = curr->children[key[0] - 'a'];

	if (nextNode && trie_remove_helper(trie, nextNode, key + 1) == 1) {
		free(nextNode->children);
        free(nextNode);

		curr->children[key[0] - 'a'] = NULL;
        curr->n_children--;

        trie->nNodes--; 

        if (curr->n_children == 0 && curr->end_of_word == 0) {
            return 1;
        }
    }

    return 0;
}

//
void trie_remove(trie_t *trie, char *key) {
    if (!trie || !key) {
		return;
	}
    trie_remove_helper(trie, trie->root, key);
}

//
void trie_free_helper(trie_t *trie, trie_node_t *curr) {
    if (!curr) {
		return;
	}

    for (int i = 0; i < trie->alphabet_size; i++) {
        if (curr->children[i] != NULL) {
            trie_free_helper(trie, curr->children[i]);
        }
    }

    if (curr->end_of_word && curr->value && trie->free_value_cb) {
        trie->free_value_cb(curr->value);
    }

    free(curr->children);
    free(curr);
    trie->nNodes--;
}

//
void trie_free(trie_t **pTrie) {
    if (!pTrie || !(*pTrie)) {
		return;
	}

    trie_t *trie = *pTrie;
    
    trie_free_helper(trie, trie->root);
    
    free(trie->alphabet);
    free(trie);
    
    *pTrie = NULL;
}

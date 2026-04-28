// SPINU Rares - 311CA

#include "commands.h"

//Helper function for checking if a word is duplicate
int dupe(struct file *f, char *word)
{
	struct list_node *node = f->keywords->head->next;
	
	for (int i = 0; i < f->keywords->size; i++) {
		char *tmp = (char *)node->data;
		if (strcmp(word, tmp) == 0) {
			return 1;
		}
		node = node->next;
	}

	return 0;
}

//Helper function for retrieving a list_node pointer from the files list
struct list_node *search_file(struct dl_list *list, char *id)
{
	struct list_node *p = list->head->next;
	for (int i = 0; i < list->size; i++) {
		struct file *f = (struct file *)p->data;
		if (strcmp(id, f->id) == 0) {
			return p;
		}
		p = p->next;
	}

	return NULL;
}

//Helper function for lexicographic sorting the keywords list
void sort(struct dl_list *list)
{
	if (!list || list->size <= 1) {
        return;
    }
	
	struct list_node *node1 = list->head->next;

	for (int i = 0; i < list->size - 1; i++) {
		struct list_node *node2 = node1->next;
		for (int j = i + 1; j < list->size; j++) {
			struct file **ptr1 = (struct file **)node1->data;
			struct file **ptr2 = (struct file **)node2->data;

			if (strcmp((*ptr1)->id, (*ptr2)->id) > 0) {
				void *temp = node1->data;
                node1->data = node2->data;
                node2->data = temp;
			}

			node2 = node2->next;
		}
		node1 = node1->next;
	}
}

//Implemented the ADD command
void add(struct system *sys, FILE *read, FILE *write)
{
	char id[101];
	int score, t;

	fscanf(read, "%s %d %d", id, &score, &t);

	//Fail, file already exists
	if (search_file(sys->list, id) != NULL) {
		char tmp[101];
		//Removing everything that is left in the buffer, so
		//nothing breaks when reading the next commands
		for (int i = 0; i < t; i++) {
			fscanf(read, "%s", tmp);
		}
		fprintf(write, "EXISTS\n");
		return;
	}

	//Creating the file that will be added
	struct file f;
	f.id = strdup(id);
	f.score = score;
	f.keywords = create_list(100 * sizeof(char));
	list_add(sys->list, sys->list->size, &f);

	struct list_node *f_node = sys->list->head->prev;
	struct file *f_ptr = (struct file *)f_node->data;

	//Creating the keywords list and adding them to it
	for (int i = 0; i < t; i++) {
		char tmp[101];
		fscanf(read, "%s", tmp);

		if (!dupe(f_ptr, tmp)) {
			list_add(f_ptr->keywords, f_ptr->keywords->size, tmp);

			trie_insert(sys->trie, tmp, f_ptr);
		}
	}
	fprintf(write, "OK\n");
}

//Implemented the DEL command
void del(struct system *sys, FILE *read, FILE *write)
{
	char id[101];
	fscanf(read, "%s", id);

	struct list_node *f_node = search_file(sys->list, id);
	if (!f_node) {
		fprintf(write, "NOT FOUND\n");
		return; //Fail
	}

	struct file *f = (struct file *)f_node->data;
	struct dl_list *keys_list = f->keywords;

	struct list_node *p = keys_list->head->next;
	for (int i = 0; i < keys_list->size; i++) {
		char *word = (char *)p->data;

		//Finding the refrences list of the current word
		struct dl_list *ref = (struct dl_list *)trie_search(sys->trie, word);

		struct list_node *ref_node = ref->head->next;
		for (int j = 0; j < ref->size; j++) {
			//Double pointer to the original file_node in the files list
			struct file **ptr = (struct file **)ref_node->data;

			//If both pointers are equal, we have found the file to remove
			if (f == *ptr) {
				list_remove(ref, ref_node);
				break;
			}
			ref_node = ref_node->next;
		}

		//If the refrence list size is 0, there are no more files that contain
		//the word, so we need to remove it from the trie
		if (ref->size == 0) {
			trie_remove(sys->trie, word);
		}
		p = p->next;
	}

	//Freeing the removed file_node
	free(f->id);
	free_list(&f->keywords);

	list_remove(sys->list, f_node);

	fprintf(write, "OK\n");
}

//Implemented the ADDKW command
void addkw(struct system *sys, FILE *read, FILE *write)
{
	char id[101], word[101];
	fscanf(read, "%s %s", id, word);

	struct list_node *f_node = search_file(sys->list, id);
	if (!f_node) {
		fprintf(write, "NOT FOUND\n");
		return; //Fail
	}

	struct file *f = (struct file *)f_node->data;

	if (!dupe(f, word)) {
		//Adding the new word to the keywords list
		list_add(f->keywords, f->keywords->size, word);

		//Inserting the new word in the trie
		trie_insert(sys->trie, word, f);
	}

	fprintf(write, "OK\n");
}

//Implemented the DELKW command
void delkw(struct system *sys, FILE *read, FILE *write)
{
	char id[101], word[101];
	fscanf(read, "%s %s", id, word);

	struct list_node *f_node = search_file(sys->list, id);
	if (!f_node) {
		fprintf(write, "NOT FOUND\n");
		return; //Fail
	}

	struct file *f = (struct file *)f_node->data;

	if (dupe(f, word)) {
		struct dl_list *ref = (struct dl_list *)trie_search(sys->trie, word);
		struct list_node *ref_node = ref->head->next;

		for (int j = 0; j < ref->size; j++) {
			//Double pointer to the original file_node in the files list,
			//as the refrences list consists of pointers to the original nodes
			struct file **ptr = (struct file **)ref_node->data;

			//If both pointers are equal, we have found the file to remove
			if (f == *ptr) {
				list_remove(ref, ref_node);
				break;
			}
			ref_node = ref_node->next;
		}

		//If the refrence list size is 0, there are no more files that contain
		//the word, so we need to remove it from the trie
		if (ref->size == 0) {
			trie_remove(sys->trie, word);
		}

		//Finding the node in the keywords list that contains the word
		//that needs to be removed, then removing it
		struct list_node *key_node = f->keywords->head->next;
        for (int i = 0; i < f->keywords->size; i++) {
            char *w = (char *)key_node->data;
            
            if (strcmp(w, word) == 0) {
                list_remove(f->keywords, key_node);
                break;
            }
            key_node = key_node->next;
        }
	}

	fprintf(write, "OK\n");
}

//Implemented the FIND command
void find(struct system *sys, FILE *read, FILE *write)
{
	char word[101];
	fscanf(read, "%s", word);

	//Getting the refrences list of the current word
	struct dl_list *ref = (struct dl_list *)trie_search(sys->trie, word);

	//If the reffrences list is empty
	if (!ref || ref->size == 0) {
        fprintf(write, "EMPTY\n");
		return;
	}

	//Sorting the refrences list, as requested
	sort(ref);

	fprintf(write, "%d ", ref->size);

	struct list_node *ref_node = ref->head->next;
	for (int i = 0; i < ref->size; i++) {
		//Getting the ids of all the files in the refrences list
		struct file **ptr = (struct file **)ref_node->data;

		fprintf(write, "%s", (*ptr)->id);
		if (i < ref->size - 1) {
			fprintf(write, " ");
		}

		ref_node = ref_node->next;
	}

	fprintf(write, "\n");
}

//Implemented the TOPK command
void topk(struct system *sys, FILE *read, FILE *write)
{
	char word[101];
	int k;
	fscanf(read, "%s %d", word, &k);

	//Getting the references list of the current word
	struct dl_list *ref = (struct dl_list *)trie_search(sys->trie, word);

	//If the references list is empty
	if (!ref || ref->size == 0) {
		fprintf(write, "EMPTY\n");
		return;
	}

	//Creating a heap for sorting the files according to score and ID
	heap_t *h = heap_create(compare_files, sizeof(struct file *));

	struct list_node *ref_node = ref->head->next;
	for (int i = 0; i < ref->size; i++) {
		struct file **ptr = (struct file **)ref_node->data;
		
		//Adding the file pointer to the heap
		heap_insert(h, ptr);

		ref_node = ref_node->next;
	}

	//Printing the number of files we are about to show
	int limit = (h->size < k) ? h->size : k;
	fprintf(write, "%d ", limit);

	//Printing the top K files from the heap
	for (int i = 0; i < limit; i++) {
		struct file **best_file_ptr = (struct file **)heap_top(h);
		struct file *best_file = *best_file_ptr;

		fprintf(write, "%s", best_file->id);
		
		//Adding a space between IDs, but not after the last one
		if (i < limit - 1) {
			fprintf(write, " ");
		}

		heap_pop(h);
	}
	fprintf(write, "\n");

	//Freeing the temporary heap
	heap_free(h);
}

//Helper function for traversing the trie and printing all the words
void print_helper(trie_node_t *node, char *word, int idx, FILE *write)
{
	if (!node) {
		return;
	}

	//If we reached the end of a word, we print the references
	if (node->end_of_word) {
		word[idx] = '\0';

		struct dl_list *ref = (struct dl_list *)node->value;

		//Sorting the references list lexicographically
		sort(ref);

		fprintf(write, "%s %d ", word, ref->size);

		struct list_node *ref_node = ref->head->next;
        for (int i = 0; i < ref->size; i++) {
			//Getting the ids of the files containing the current word
            struct file **ptr = (struct file **)ref_node->data;
            fprintf(write, "%s", (*ptr)->id);
			if (i < ref->size - 1) {
				fprintf(write, " ");
			}

            ref_node = ref_node->next;
        }
        fprintf(write, "\n");
	}

	//Traversing every child of the current node
	for (int i = 0; i < 26; i++) {
        if (node->children[i] != NULL) {
            word[idx] = 'a' + i;
            
            print_helper(node->children[i], word, idx + 1, write);
        }
    }
}

//Implemented the PRINT command
void print(struct system *sys, FILE *read, FILE *write)
{
	(void)read;
	if (!sys || sys->trie->size == 0) {
		fprintf(write, "EMPTY\n");
		return;
	}

	char word[101];
    print_helper(sys->trie->root, word, 0, write);

}

//Function for running all the commands
void run_commands(int Q, struct system *sys, FILE *read, FILE *write)
{
    char command[10];

    for (int i = 0; i < Q; i++) {
        fscanf(read, "%s", command);

        if (strcmp(command, "ADD") == 0) {
            add(sys, read, write);
        } else if (strcmp(command, "DEL") == 0) {
            del(sys, read, write);
        } else if (strcmp(command, "ADDKW") == 0) {
            addkw(sys, read, write);
        } else if (strcmp(command, "DELKW") == 0) {
            delkw(sys, read, write);
        } else if (strcmp(command, "FIND") == 0) {
            find(sys, read, write);
        } else if (strcmp(command, "TOPK") == 0) {
            topk(sys, read, write);
        } else if (strcmp(command, "PRINT") == 0) {
            print(sys, read, write);
        }
    }
}

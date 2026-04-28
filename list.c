// SPINU Rares - 311CA

#include "list.h"

//Function for creating a new list
struct dl_list *create_list(int data_size)
{
	struct dl_list *list = malloc(sizeof(struct dl_list));
	if (!list) {
		return NULL;
	}

	//Initializing the sentinel node
	list->head = malloc(sizeof(struct list_node));
	if (!list->head) {
		free(list);
		return NULL;
	}
	list->head->next = list->head;
	list->head->prev = list->head;
	list->head->data = NULL;

	list->size = 0;
	list->data_size = data_size;

	return list;
}

//Function for adding a new element to the list
void list_add(struct dl_list *list, int n, void *data)
{
	if (!list) {
		return;
	}

	if (n >= list->size) {
		n = list->size;
	}

	struct list_node *new_node = malloc(sizeof(struct list_node));
	if (!new_node) {
		return;
	}

	new_node->data = malloc(list->data_size);
	if (!new_node->data) {
		free(new_node);
		return;
	}
	memcpy(new_node->data, data, list->data_size);

	struct list_node *p = list->head->next;
	for (int i = 0; i < n; i++) {
		p = p->next;
	}

	new_node->next = p;
	new_node->prev = p->prev;
	p->prev->next = new_node;
	p->prev = new_node;

	list->size++;
}

//Function for removing a node from the list
void list_remove(struct dl_list *list, struct list_node *node)
{
	if (!list || !node || list->size == 0) {
		return;
	}

	//Removing the links from the node
	node->prev->next = node->next;
	node->next->prev = node->prev;

	list->size--;

	//Freeing the memory of the deleted node
	if (node->data) {
		free(node->data);
	}
	free(node);
}

//Function for freeing lists
void free_list(struct dl_list **list)
{

	if (!list || !*list) {
		return;
	}

	struct list_node *curr_node = (*list)->head;
	struct list_node *next_node;

	for (int i = 0; i <= (*list)->size; i++) {
		next_node = curr_node->next;

		free(curr_node->data);
		free(curr_node);

		curr_node = next_node;
	}

	free(*list);
	*list = NULL;
}

// SPINU Rares - 311CA

#ifndef list_h
#define list_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct list_node {
	void *data;
	struct list_node *next, *prev;
};

struct dl_list {
	struct list_node *head;
	int size;
	int data_size;
};

struct file {
	char *id;
	int score;
	struct dl_list *keywords;
};

struct dl_list *create_list(int data_size);
void list_add(struct dl_list *list, int n, void *data);
void free_list(struct dl_list **list);
void list_remove(struct dl_list *list, struct list_node *node);

#endif

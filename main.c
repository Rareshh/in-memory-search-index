// SPINU Rares - 311CA

#include <stdio.h>
#include <stdlib.h>
#include "list.h"
#include "trie.h"
#include "commands.h"

int main()
{
	//Opening the .in and .out files
	FILE *read = fopen("indexare.in", "r");
	if (!read) {
		fprintf(stderr, "File opening error\n");
		return -1;
	}

	FILE *write = fopen("indexare.out", "w");
	if (!write) {
		fprintf(stderr, "File opening error\n");
		return -1;
	}

	//Initializing the Files_List and the Trie
	struct system *sys = init_system();

	int Q;
	fscanf(read, "%d", &Q);

	//Reading and executing the commands
	run_commands(Q, sys, read, write);

	//Freeing everything and closing the files
	fclose(read);
	fclose(write);

	free_system(sys);

	return 0;
}
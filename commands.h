// SPINU Rares - 311CA

#ifndef commands_h
#define commands_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "system.h"
#include "heap.h"

void run_commands(int Q, struct system *sys, FILE *read, FILE *write);
void add(struct system *sys, FILE *read, FILE *write);
void del(struct system *sys, FILE *read, FILE *write);
void addkw(struct system *sys, FILE *read, FILE *write);
void delkw(struct system *sys, FILE *read, FILE *write);
void find(struct system *sys, FILE *read, FILE *write);
void topk(struct system *sys, FILE *read, FILE *write);
void print(struct system *sys, FILE *read, FILE *write);

#endif

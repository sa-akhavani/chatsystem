#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include "tracker.h"


void add_user(struct user* head, char* username, char* ip, char* port);

int find_user(struct user* head, char* username);
void clear_all(struct user* head);
void print_all(struct user* head);

#endif
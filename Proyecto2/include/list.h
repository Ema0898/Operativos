#ifndef LIST_H
#define LIST_H

void print_list(node_t *head);
void add_end(node_t *head, int val);
void add_start(node_t **head, int val);
int remove_start(node_t **head);
int remove_last(node_t *head);
int remove_by_index(node_t **head, int n);
int get_size();

#endif
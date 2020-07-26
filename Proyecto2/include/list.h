#ifndef _LIST_H
#define _LIST_H

struct node
{
    void *data;
    struct node *next;
};

typedef struct node *llist;

llist *llist_create(void *data);
void llist_free(llist *list);
void llist_push(llist *list, void *data);
void *llist_pop(llist *list);
void llist_print(llist *list);
void llist_insert_end(llist *list, void *data);
int llist_insert_by_index(llist *list, void *data, int Id);
void llist_insert_scheduler_attribute(llist *list, void *data, int comparation);
void *llist_get_by_index(llist *list, int index);
int llist_get_size(llist *list);
int llist_remove_by_index(llist *list, int Id);
void *llist_get_winner(llist *list, int winner);
int llist_get_alien_index(llist *list, int id);

#endif
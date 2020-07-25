#ifndef _SCHEDULERS_H
#define _SCHEDULERS_H

void priority_scheduler(llist *aliens, void *data);
void SJF_scheduler(llist *aliens, void *data);
void FIFO_scheduler(llist *aliens, void *data);
void *lottery_winner(llist *aliens);
void lottery_scheduler(llist *aliens, void *data);
#endif
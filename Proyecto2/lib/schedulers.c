#include <sys/time.h>
#include <structs.h>
#include <list.h>
#include <stdio.h>
#include <stdlib.h>
#include <list.h>
#include <time.h>

/* Variables */
struct timeval tic, toc;
int lottery_number_now = 1;
time_t t;
llist *possible_numbers = NULL;

/* Priority scheduler */
void priority_scheduler(llist *aliens, void *data)
{
  /* Method that inserts to the list depending on the priority */
  llist_insert_scheduler_attribute(aliens, data, 0);
}

void SJF_scheduler(llist *aliens, void *data)
{
  /* Method that inserts to the list depending on the velocity */
  llist_insert_scheduler_attribute(aliens, data, 1);
}

void FIFO_scheduler(llist *aliens, void *data)
{
  /* Insert in order */
  llist_insert_end(aliens, data);
}

/* Check the winner ticket */
void *lottery_winner(llist *aliens)
{
  /* Generate random with tickets and select the winner */
  alien *winner = NULL;
  srand((unsigned)time(&t));
  int bingo;
  int alien_index;

  while (winner == NULL)
  {
    bingo = rand() % llist_get_size(possible_numbers);
    alien_index = *((int *)llist_get_by_index(possible_numbers, bingo));
    winner = (alien *)llist_get_winner(aliens, alien_index);
  }
  llist_remove_by_index(possible_numbers, bingo);
  return winner;
}
/*Give tickets*/
void lottery_scheduler(llist *aliens, void *data)
{
  /*insert an alien and give it tickets depending on the priority = 0 || 1*/
  if (possible_numbers == NULL)
  {
    possible_numbers = llist_create(NULL);
  }

  alien *new_alien = (alien *)data;

  int *tickets_to_give = (int *)malloc(sizeof(int) * 2);

  for (int i = 0; i < 2 - new_alien->priority; i++)
  {
    *(tickets_to_give + i) = lottery_number_now;
    llist_insert_end(possible_numbers, (void *)(tickets_to_give + i));
    lottery_number_now++;
  }

  new_alien->lottery_numbers = tickets_to_give;

  llist_insert_end(aliens, data);
}
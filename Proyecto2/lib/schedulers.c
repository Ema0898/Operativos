#include <sys/time.h>
#include <structs.h>
#include <list.h>
#include <stdio.h>
#include <stdlib.h>
#include <list.h>
#include <time.h>

struct timeval tic, toc;
int lottery_number_now = 1;
time_t t;
llist *possible_numbers = NULL;

void priority_scheduler(llist *aliens, void *data)
{
  llist_insert_scheduler_attribute(aliens, data, 0);
}

void SJF_scheduler(llist *aliens, void *data)
{
  llist_insert_scheduler_attribute(aliens, data, 1);
}

void FIFO_scheduler(llist *aliens, void *data)
{
  llist_insert_end(aliens, data);
}

void *lottery_winner(llist *aliens)
{
  // Genera un random con los tiquetes dados y se extrae al ganador de la lista
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
  llist_remove_by_index(possible_numbers, alien_index);
  return winner;
}

void lottery_scheduler(llist *aliens, void *data)
{
  // inserta un alien y le da tiquetes dependiendo de la prioridad = 0 || 1
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
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

int roundRobin(alien *alien, int quantum)
{

  gettimeofday(&toc, NULL);
  tic = alien->last_update;
  float time_difference = (double)(toc.tv_usec - tic.tv_usec) / 1000000 + (double)(toc.tv_sec - tic.tv_sec);

  alien->last_update = toc;
  alien->accumulator += time_difference;
  alien->progress = alien->duration / alien->accumulator;

  if (alien->progress == 1)
  {
    printf("Termine el trabajo!\n");
    return 1;
  }
  else if (alien->accumulator >= quantum)
  {
    printf("Se me acabo el quantum!\n");
    return 2;
  }

  return 0;
}

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

void lottery_winner(llist *aliens)
{
  // Genera un random con los tiquetes dados y se extrae al ganador de la lista
  alien *winner = NULL;
  srand((unsigned)time(&t));

  while (winner == NULL)
  {
    int bingo = rand() % llist_get_size(possible_numbers);
    winner = (alien *)llist_get_winner(aliens, *((int *)llist_get_by_index(possible_numbers, bingo)));
    llist_remove_by_index(possible_numbers, bingo);
  }
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

  for (int i = 0; i <= new_alien->priority; i++)
  {
    printf("Dando el tiquete numero %d al alien\n", lottery_number_now);
    *(tickets_to_give + i) = lottery_number_now;
    llist_insert_end(possible_numbers, (void *)(tickets_to_give + i));
    lottery_number_now++;
  }

  new_alien->lottery_numbers = tickets_to_give;

  llist_insert_end(aliens, data);
}
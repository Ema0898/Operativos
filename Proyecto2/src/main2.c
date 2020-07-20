#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <list.h>
#include <sys/time.h>
#include <structs.h>
#include <stddef.h>
#include <schedulers.h>

#define OFFSETOF(TYPE, ELEMENT) ((size_t)&(((TYPE *)0)->ELEMENT))

#define ROUND_ROBIN 0

int main(){
  llist* aliens = llist_create(NULL);

  alien* new_alien =  (alien*)malloc(sizeof(alien));
  gettimeofday(&new_alien->work_init_time, NULL);
  new_alien->last_update = new_alien->work_init_time;
  new_alien->progress = 50;
  new_alien->duration = 7;
  new_alien->working = 4;
  new_alien->accumulator = 1;
  new_alien->priority = 0;
  lottery_scheduler(aliens, (void*)new_alien);

  alien* new_alien2 =  (alien*)malloc(sizeof(alien));
  gettimeofday(&new_alien2->work_init_time, NULL);
  new_alien2->last_update = new_alien2->work_init_time;
  new_alien2->progress = 50;
  new_alien2->duration = 5;
  new_alien2->working = 5;
  new_alien2->accumulator = 1;
  new_alien2->priority = 0;
  lottery_scheduler(aliens, (void*)new_alien2);

  alien* new_alien3 =  (alien*)malloc(sizeof(alien));
  gettimeofday(&new_alien3->work_init_time, NULL);
  new_alien3->last_update = new_alien3->work_init_time;
  new_alien3->progress = 50;
  new_alien3->duration = 15;
  new_alien3->working = 5;
  new_alien3->accumulator = 1;
  new_alien3->priority = 1;
  lottery_scheduler(aliens, (void*)new_alien3);

  llist_print(aliens);

  printf("Este alien tiene los tiquetes %d y %d\n", *new_alien3->lottery_numbers, *(new_alien3->lottery_numbers + 1));

  lottery_winner(aliens);
  
  printf("List Size: %d\n", llist_get_size(aliens));

  
  int bride_method = ROUND_ROBIN;

  if(bride_method == ROUND_ROBIN){
    
  }
}
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <list.h>
#include <sys/time.h>
#include <structs.h>

#define ROUND_ROBIN 0

int main(){
  llist* aliens = llist_create(NULL);

  alien* new_alien =  (alien*)malloc(sizeof(alien));
  gettimeofday(&new_alien->work_init_time, NULL);
  new_alien->last_update = new_alien->work_init_time;
  new_alien->progress = 50;
  new_alien->duration = 5;
  new_alien->working = 2;
  new_alien->accumulator = 1;
  llist_insert_end(aliens, (void*)new_alien);

  alien* new_alien2 =  (alien*)malloc(sizeof(alien));
  gettimeofday(&new_alien2->work_init_time, NULL);
  new_alien2->last_update = new_alien2->work_init_time;
  new_alien2->progress = 50;
  new_alien2->duration = 7;
  new_alien2->working = 2;
  new_alien2->accumulator = 1;
  llist_insert_end(aliens, (void*)new_alien2);

  alien* new_alien3 =  (alien*)malloc(sizeof(alien));
  gettimeofday(&new_alien3->work_init_time, NULL);
  new_alien3->last_update = new_alien3->work_init_time;
  new_alien3->progress = 50;
  new_alien3->duration = 6;
  new_alien3->working = 2;
  new_alien3->accumulator = 1;
  llist_insert_end(aliens, (void*)new_alien3);

  llist_print(aliens);
  
  int bride_method = ROUND_ROBIN;

  if(bride_method == ROUND_ROBIN){

  }
}
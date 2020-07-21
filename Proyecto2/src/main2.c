#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <list.h>
#include <sys/time.h>
#include <structs.h>
#include <schedulers.h>
#include <algorithms.h>

#define ROUND_ROBIN 0

int main(){
  llist* aliens = llist_create(NULL);
  llist* bridge = llist_create(NULL);

  alien* new_alien =  (alien*)malloc(sizeof(alien));
  gettimeofday(&new_alien->work_init_time, NULL);
  new_alien->last_update = new_alien->work_init_time;
  new_alien->progress = 50;
  new_alien->duration = 2;
  new_alien->working = 4;
  new_alien->accumulator = 0;
  new_alien->priority = 0;
  new_alien->weight =  15;
  lottery_scheduler(aliens, (void*)new_alien);

  alien* new_alien2 =  (alien*)malloc(sizeof(alien));
  gettimeofday(&new_alien2->work_init_time, NULL);
  new_alien2->last_update = new_alien2->work_init_time;
  new_alien2->progress = 50;
  new_alien2->duration = 2;
  new_alien2->working = 5;
  new_alien2->accumulator = 0;
  new_alien2->priority = 0;
  new_alien2->weight =  10;
  lottery_scheduler(aliens, (void*)new_alien2);

  alien* new_alien3 =  (alien*)malloc(sizeof(alien));
  gettimeofday(&new_alien3->work_init_time, NULL);
  new_alien3->last_update = new_alien3->work_init_time;
  new_alien3->progress = 50;
  new_alien3->duration = 2; 
  new_alien3->working = 5;
  new_alien3->accumulator = 0;
  new_alien3->priority = 1;
  new_alien3->weight =  20;
  lottery_scheduler(aliens, (void*)new_alien3);

  // llist_print(aliens);

  // llist_remove_by_index(aliens, 0);

  // lottery_winner(aliens);

  int *weight_now = (int*)malloc(sizeof(int));
  *weight_now = 0;
  algs_params* params = (algs_params*)malloc(sizeof(algs_params));
  params->north = aliens;
  params->south = aliens;
  params->bridge = bridge;
  params->amount_to_pass = 5;
  params->bridge_weight = 100; 
  params->weight_now = weight_now; 
  
  Y_algorithm(params);

  int bride_method = ROUND_ROBIN;

  if(bride_method == ROUND_ROBIN){
    
  }
}
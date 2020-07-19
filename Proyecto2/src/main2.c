#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <list.h>
#include <sys/time.h>
#include <structs.h>

#define ROUND_ROBIN 0

int main(){
  llist* aliens = llist_create(NULL);

  alien* new_alien =  malloc(sizeof(alien));
  gettimeofday(&new_alien->work_init_time, NULL);
  new_alien->last_update = new_alien->work_init_time;
  new_alien->progress = 50;
  new_alien->duration = 5;
  new_alien->working = 2;
  new_alien->accumulator = 1;
  llist_push(aliens, (void*)new_alien);

  alien* new_alien2 =  malloc(sizeof(alien));
  gettimeofday(&new_alien2->work_init_time, NULL);
  new_alien2->last_update = new_alien2->work_init_time;
  new_alien2->progress = 50;
  new_alien2->duration = 7;
  new_alien2->working = 2;
  new_alien2->accumulator = 1;
  llist_push(aliens, (void*)new_alien2);

  alien* new_alien3 =  malloc(sizeof(alien));
  gettimeofday(&new_alien3->work_init_time, NULL);
  new_alien3->last_update = new_alien3->work_init_time;
  new_alien3->progress = 50;
  new_alien3->duration = 6;
  new_alien3->working = 2;
  new_alien3->accumulator = 1;
  llist_push(aliens, (void*)new_alien3);

  // alien* new_alien3;

  // bridge *bridge1 = malloc(sizeof(bridge));
  // bridge1->weight = 10;
  // llist_push(aliens, (void*)bridge1);

  // bridge *bridge2 = malloc(sizeof(bridge));
  // bridge2->weight = 1;
  // llist_push(aliens, (void*)bridge2);

  // bridge *bridge3 = malloc(sizeof(bridge));
  // bridge3->weight = 5;
  // llist_push(aliens, (void*)bridge3);

  printf("Size antes: %d\n", llist_get_size(aliens));

  // int poppedAlien = llist_remove_by_index(aliens, 1);

  alien* test = (alien*)llist_get_by_index(aliens, 2);

  if(test != NULL) {
    printf("Size despues: %d\n", llist_get_size(aliens));
    printf("Hola, soy el alien: %f\n", test->duration);
  }
  
  int bride_method = ROUND_ROBIN;

  if(bride_method == ROUND_ROBIN){

  }
}

#include <structs.h>
#include <list.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>


void bridge_list_update(algs_params *params) {
  struct timeval tic, toc;
  float time_difference;
  int list_size = llist_get_size(params->bridge);
  alien *alien_now;

  for(int i = 0; i < list_size; i++) {
    alien_now = (alien*)llist_get_by_index(params->bridge, i);
    gettimeofday(&toc, NULL);
    tic = alien_now -> last_update;
    time_difference = (double)(toc.tv_usec - tic.tv_usec) / 1000000 + (double)(toc.tv_sec - tic.tv_sec);
    alien_now->last_update = toc;
    alien_now->accumulator += time_difference;
    alien_now->progress = alien_now->accumulator/alien_now->duration;
    if(alien_now->progress >= 1) {
      *params->weight_now -= alien_now->weight; 
      llist_remove_by_index(params->bridge, i);
      break;
    }
  }
}

void Y_algorithm(algs_params *params){

  int turn = 0;
  int i;
  alien* alien_to_move;

  while(1) {

    for(i = 0; i < params->amount_to_pass; i++) {
      if(llist_get_size(params->north) == 0 && llist_get_size(params->south) == 0) {
        sleep(1);
      }
      if(turn == 0) {
        if(llist_get_size(params->north) == 0) {
          break;
        };
        alien_to_move = (alien*)llist_pop(params->north);
      } else {
        if(llist_get_size(params->south) == 0) {
          break;
        };
        alien_to_move = (alien*)llist_pop(params->south);
      }
      while(*params->weight_now + alien_to_move->weight > params->bridge_weight) {
        bridge_list_update(params);
        continue;
      }
      *params->weight_now += alien_to_move->weight;
      alien_to_move->working = 1;
      gettimeofday(&alien_to_move->last_update, NULL);
      llist_insert_end(params->bridge, alien_to_move);
    }
    while(llist_get_size(params->bridge) > 0){
      bridge_list_update(params);
    }

    turn = !turn;
  }

}
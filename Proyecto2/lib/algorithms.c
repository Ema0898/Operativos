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
      printf("Termino de pasar el que faltaba\n");
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

void survival_algorithm(algs_params *params){
  // Toma las dos listas y solo pasa 1 de cada una al algoritmo Y
  params->amount_to_pass = 1;
  Y_algorithm(params);
}

void semaphore_algorithm(algs_params *params){
  int turn = 0;
  int i;

  struct timeval tic2, toc2;
  float time_difference2, accumulator;

  float sem_north_time = 2, sem_south_time = 4;

  params->amount_to_pass = 1;

  alien* alien_to_move;

  while(1) {
    gettimeofday(&tic2, NULL);

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
      if(*params->weight_now + alien_to_move->weight <= params->bridge_weight) {
        *params->weight_now += alien_to_move->weight;
        alien_to_move->working = 1;
        gettimeofday(&alien_to_move->last_update, NULL);
        llist_insert_end(params->bridge, alien_to_move);
      }
    }

    gettimeofday(&toc2, NULL);
    time_difference2 = (double)(toc2.tv_usec - tic2.tv_usec) / 1000000 + (double)(toc2.tv_sec - tic2.tv_sec);
    accumulator += time_difference2;

    bridge_list_update(params);

    if(turn == 0) {
      if(accumulator >= sem_north_time) {
        printf("Cambio de lado al sur\n");
        while(llist_get_size(params->bridge) > 0){
          bridge_list_update(params);
        }
        turn = !turn;
        accumulator = 0;
      }
    } else {
      if(accumulator >= sem_south_time) {
        printf("Cambio de lado al norte\n");
        while(llist_get_size(params->bridge) > 0){
          bridge_list_update(params);
        }
        turn = !turn;
        accumulator = 0;
      }
    }
  }
}
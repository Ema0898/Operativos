#include <sys/time.h>
#include <structs.h>
#include <list.h>
#include <stdio.h>

struct timeval tic, toc;

int roundRobin(alien* alien, int quantum){

  gettimeofday(&toc, NULL);
  tic = alien -> last_update;
  float time_difference = (double)(toc.tv_usec - tic.tv_usec) / 1000000 + (double)(toc.tv_sec - tic.tv_sec);

  alien->last_update = toc;
  alien->accumulator += time_difference;
  alien->progress = alien->duration / alien->accumulator;

  if(alien->progress == 1) {
    printf("Termine el trabajo!\n");
    return 1;
  } else if(alien->accumulator >= quantum){
    printf("Se me acabo el quantum!\n");
    return 2;
  }

  return 0;
}
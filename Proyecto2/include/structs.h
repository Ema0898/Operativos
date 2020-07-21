#ifndef _STRUCTS_H
#define _STRUCTS_H

#include <sys/time.h>
#include <pthread.h>
#include <list.h>

/* structs used for bridge variables */

typedef struct
{
  int weight;
  int length;
} bridge;

typedef struct
{
  float x;
  float y;
} point;

typedef struct
{
  float priority; // **alien
  float duration; // *(*alien + 4)
  struct timeval work_init_time;
  struct timeval last_update;
  float progress;
  float accumulator;
  short working;
  int *lottery_numbers;
  point pos;
  pthread_t *thread;
  int type;
} alien;


typedef struct
{
  llist *north;
  llist *south;
  llist *bridge;
  int   amount_to_pass;
  int   bridge_weight;
  int  *weight_now;
} algs_params;

#endif

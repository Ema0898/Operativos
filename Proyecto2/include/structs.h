#ifndef _STRUCTS_H
#define _STRUCTS_H

#include <sys/time.h>
#include <lpthread.h>
#include <list.h>

#define ROUND_ROBIN 0
#define PRIORITY    1
#define FIFO        2
#define SJF         3
#define LOTTERY     4

/* structs used for bridge variables */

typedef struct
{
  int weight;
  int length;
  int bridge_type;
  int quantum;
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
  int weight;
  point pos;
  lpthread_t *thread;
  int type;
  float velocity;
  int id;
} alien;

typedef struct
{
  llist *north;
  llist *south;
  llist *bridge;
  int   amount_to_pass;
  int   bridge_weight;
  int  *weight_now;
  int   turn;
  bridge* bridge_struct;
} algs_params;

#endif
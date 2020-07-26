#ifndef _STRUCTS_H
#define _STRUCTS_H

#include <sys/time.h>
#include <lpthread.h>
#include <list.h>

#define ROUND_ROBIN 0
#define PRIORITY 1
#define FIFO 2
#define SJF 3
#define LOTTERY 4

#define Y_ALGORITHM 0
#define SEMAPHORE_ALGORITM 1
#define SURVIVAL_ALGORITHM 2

/* structs used for bridge variables */

typedef struct
{
  int weight;
  int length;
  int bridge_type;
  int quantum;
  int amount_north;
  int amount_south;
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
  int rr_quantum;
} alien;

typedef struct
{
  llist *north;
  llist *south;
  llist *bridge;
  int amount_to_pass;
  int bridge_weight;
  int *weight_now;
  int turn;
  bridge *bridge_struct;
  int north_time;
  int south_time;
  int *turn_semaphore;
  int *aliens_count_north;
  int *aliens_count_south;
} algs_params;

typedef struct
{
  int y_algorithm_confg;
  int semaphore_north_confg;
  int semaphore_south_confg;
  int quantum_confg;
  int length_confg;
  int weight_confg;
  int scheduler_confg;
  int algorithm_confg;
  int amount_north_confg;
  int amount_south_confg;

} configurable;

#endif
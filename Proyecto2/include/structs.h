#ifndef _STRUCTS_H
#define _STRUCTS_H
#include <sys/time.h>

/* structs used for bridge variables */

typedef struct
{
  int weight;
  int length;
} bridge;

typedef struct
{
  float   priority; // **alien
  float   duration; // *(*alien + 4)
  struct timeval work_init_time;
  struct timeval last_update;
  float   progress;
  float   accumulator;
  short   working;
  int *lottery_numbers;
} alien;

typedef struct
{
  float x;
  float y;
} point;

#endif
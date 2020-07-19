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
  struct timeval work_init_time;
  struct timeval last_update;
  float   progress;
  float   duration;
  float   accumulator;
  short   working;
} alien;

typedef struct
{
  float x;
  float y;
} point;

#endif
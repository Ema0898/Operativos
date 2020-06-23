#ifndef _STRUCTS_H
#define _STRUCTS_H

/* structs used for buffer messages and global variables */

typedef struct
{
  int pid;
  int magic_number;
  int is_used;
  char date[50];  
} message;

typedef struct
{
  int producers;
  int consumers;
  int size;
  int kill;
  double waiting_time;
  double blocked_time;
  double user_time;
} global_variables;

#endif
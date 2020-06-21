#ifndef _STRUCTS_H
#define _STRUCTS_H

/* structs used for buffer messages and global variables */

typedef struct
{
  int pid;
  int magic_number;
  char date[50];
  char hour[50];
} message;

typedef struct
{
  int producers;
  int consumers;
  int size;
  int kill;
} global_variables;

#endif
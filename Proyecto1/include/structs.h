#ifndef _STRUCTS_H
#define _STRUCTS_H

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
} global_variables;

#endif
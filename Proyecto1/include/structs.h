#ifndef _STRUCTS_H
#define _STRUCTS_H

typedef struct
{
  int pid;
  int magic_numer;
  char date[50];
  char hour[50];
} message;

typedef struct
{
  int producers;
  int consumers;
} global_variables;

#endif
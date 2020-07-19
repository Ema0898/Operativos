#ifndef _STRUCTS_H
#define _STRUCTS_H

/* structs used for bridge variables */

typedef struct
{
  int weight;
  int length;
} bridge;

typedef struct node
{
  int val;
  struct node *next;
} node_t;

typedef struct
{
  float x;
  float y;
} point;

#endif
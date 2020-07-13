#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

void load_map(int map[24][46])
{
  FILE *file = fopen("assets/map/map1.map", "r");

  char ch;
  int i = 0;
  int j = 0;
  while ((ch = fgetc(file)) != EOF)
  {
    if (ch == '\n')
    {
      i += 1;
      j = 0;
    }
    else if (ch == '-')
    {
      (map[i][j]) = 0;
      j += 1;
    }
    else if (ch == 'x')
    {
      (map[i][j]) = 1;
      j += 1;
    }
    else if (ch == 'o')
    {
      (map[i][j]) = 2;
      j += 1;
    }
    else if (ch == 'p')
    {
      (map[i][j]) = 3;
      j += 1;
    }
  }
  fclose(file);
}
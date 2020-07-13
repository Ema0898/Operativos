#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

const int X_SIZE = 46;
const int Y_SIZE = 24;

void load_map(int map[Y_SIZE][X_SIZE]);

int main()
{
  int map[24][46];

  load_map(map);

  for (int i = 0; i < 24; ++i)
  {
    for (int j = 0; j < 46; ++j)
    {
      printf("%d", map[i][j]);
      printf(" ");
    }
    printf("\n");
  }
  return 0;
}

void load_map(int map[Y_SIZE][X_SIZE])
{
  FILE *file = fopen("map1.map", "r");

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
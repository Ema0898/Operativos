#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <string.h>
#include <time.h>
#include <math.h>

int load_map(int map[24][46])
{
  FILE *file = fopen("../assets/map/map1.map", "r");

  if (file == NULL)
  {
    return 0;
  }

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

  return 1;
}

/* split function */
int split(char *str, char c, char ***arr)
{
  int count = 1;
  int token_len = 1;
  int i = 0;
  char *p;
  char *t;

  p = str;
  while (*p != '\0')
  {
    if (*p == c)
      count++;
    p++;
  }

  *arr = (char **)malloc(sizeof(char *) * count);
  if (*arr == NULL)
    exit(1);

  p = str;
  while (*p != '\0')
  {
    if (*p == c)
    {
      (*arr)[i] = (char *)malloc(sizeof(char) * token_len);
      if ((*arr)[i] == NULL)
        exit(1);

      token_len = 0;
      i++;
    }
    p++;
    token_len++;
  }
  (*arr)[i] = (char *)malloc(sizeof(char) * token_len);
  if ((*arr)[i] == NULL)
    exit(1);

  i = 0;
  p = str;
  t = ((*arr)[i]);
  while (*p != '\0')
  {
    if (*p != c && *p != '\0')
    {
      *t = *p;
      t++;
    }
    else
    {
      *t = '\0';
      i++;
      t = ((*arr)[i]);
    }
    p++;
  }

  return count;
}

/* concats two strings */
char *concat(const char *s1, const char *s2)
{
  char *result = (char *)malloc(strlen(s1) + strlen(s2) + 1);

  strcpy(result, s1);
  strcat(result, s2);
  return result;
}

/* check if program is executed from bin directory */
int check_bin_dir(void)
{
  char cwd[PATH_MAX];
  if (getcwd(cwd, sizeof(cwd)) == NULL)
  {
    printf("Can't get current directory\n");
    exit(0);
  }

  char **array;
  char *cont = concat(cwd, "/");
  int size = split(cont, '/', &array);

  int result = 0;

  if (!strcmp(array[size - 2], "bin"))
  {
    result = 1;
  }

  for (int i = 0; i < size; ++i)
  {
    free(array[i]);
  }

  free(array);
  free(cont);

  return result;
}

/* check if a string is a number */
int is_number(char *text)
{
  int j;
  j = strlen(text);
  while (j--)
  {
    if (text[j] > 47 && text[j] < 58)
      continue;

    return 0;
  }
  return 1;
}

int valdite_args(int argc, char *argv[], int *medium)
{
  if (argc != 3)
  {
    printf("Usage: ./main <operation_mode (manual/automatic)> <time_medium>\n");
    return 0;
  }

  if (strcmp(argv[1], "manual") != 0 && strcmp(argv[1], "automatic") != 0)
  {
    printf("Please insert a correct operation mode\n");
    return 0;
  }

  if (!is_number(argv[2]))
  {
    printf("Please Insert a Correct Time Medium\n");
    return 0;
  }

  if (!strcmp(argv[1], "automatic"))
  {
    *medium = atoi(argv[2]);
  }

  return 1;
}

int generate_random(int upper, int lower)
{
  return (rand() % (upper - lower + 1)) + lower;
}

/* Exponential Distribution */
double ran_expo(double lambda)
{
  double u;
  u = rand() / (RAND_MAX + 1.0);
  return -log(1 - u) / lambda;
}

void generate_probability(int *a_probability, int *b_probability, int *percentages)
{
  int a_alpha = percentages[1];
  int a_beta = percentages[2];

  int b_alpha = percentages[4];
  int b_beta = percentages[5];

  if ((a_alpha + a_beta) <= 10)
  {
    for (int i = 0; i < a_alpha; ++i)
    {
      a_probability[i] = 1;
    }

    for (int i = a_alpha; i < (a_alpha + a_beta); ++i)
    {
      a_probability[i] = 2;
    }
  }

  if ((b_alpha + b_beta) <= 10)
  {
    for (int i = 0; i < b_alpha; ++i)
    {
      b_probability[i] = 1;
    }

    for (int i = b_alpha; i < (b_alpha + b_beta); ++i)
    {
      b_probability[i] = 2;
    }
  }
}
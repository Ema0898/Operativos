#include <unistd.h>
#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>

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

int isNumber(char *text)
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

int floatNum(char *s)
{
  const char *ptr = s;
  double x = strtod(ptr, &s);

  //check if converted to long int
  if (*s == 0)
  {
    return 1;
  }

  return 0;
}

int main()
{
  // char cwd[PATH_MAX];
  // if (getcwd(cwd, sizeof(cwd)) == NULL)
  // {
  //   printf("Can't get current directory\n");
  //   exit(0);
  // }

  // char **array;
  // int size = split(cwd, '/', &array);

  // for (int i = 0; i < size; ++i)
  // {
  //   printf("%s\n", array[i]);
  // }

  // if (!strcmp(array[size - 1], "bin"))
  // {
  //   printf("I'm running from bin directory!\n");
  // }
  // else
  // {
  //   printf("I'm running from root directory!\n");
  // }

  // char *hola = "1234a";

  // if (isNumber(hola))
  // {
  //   printf("It's a number\n");
  // }
  // else
  // {
  //   printf("Isn't a number\n");
  // }
  char *hola = "43434.54343434";

  if (floatNum(hola) == 1)
  {
    printf("It's a float number!\n");
  }
  else
  {
    printf("Isn't a float number!\n");
  }

  return 0;
}
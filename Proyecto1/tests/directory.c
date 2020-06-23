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

/**
 * Prints text in color
 * 1: Bold Red
 * 2: Bold Blue
 * 3: Bold Green
 * 4: Bold Yellow
 * 5: Bold Magenta
 * 6: Bold Cyan
*/
void printc(char *msg, int color)
{
  if (color == 1)
  {
    printf("\033[1;31m");
    printf("%s", msg);
    printf("\033[0m");
  }
  else if (color == 2)
  {
    printf("\033[1;34m");
    printf("%s", msg);
    printf("\033[0m");
  }
  else if (color == 3)
  {
    printf("\033[1;32m");
    printf("%s", msg);
    printf("\033[0m");
  }
  else if (color == 4)
  {
    printf("\033[1;33m");
    printf("%s", msg);
    printf("\033[0m");
  }
  else if (color == 5)
  {
    printf("\033[1;35m");
    printf("%s", msg);
    printf("\033[0m");
  }
  else if (color == 6)
  {
    printf("\033[1;36m");
    printf("%s", msg);
    printf("\033[0m");
  }
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
  // char *hola = "43434.54343434";

  // if (floatNum(hola) == 1)
  // {
  //   printf("It's a float number!\n");
  // }
  // else
  // {
  //   printf("Isn't a float number!\n");
  // }

  /* Message Written by Producer */
  printc("##########################\n", 2);
  printc("      Message Written\n", 6);
  printc("Index:", 2);
  printc("\t\t\t2\n", 3);
  printc("Alive Consumers:", 2);
  printc("\t2\n", 3);
  printc("Alive Producers:", 2);
  printc("\t2\n", 3);
  printc("##########################\n", 2);

  /* Message Read by Consumer */
  printc("#################################################\n", 2);
  printc("      Message Read\n", 6);
  printc("Index:", 2);
  printc("\t\t\t2\n", 3);
  printc("Alive Consumers:", 2);
  printc("\t2\n", 3);
  printc("Alive Producers:", 2);
  printc("\t2\n", 3);
  printc("Date:", 2);
  printc("\t\t\tSat May 23 10:41:47 2020\n", 3);
  printc("PID:", 2);
  printc("\t\t\t123\n", 3);
  printc("Magic Number:", 2);
  printc("\t\t2\n", 3);
  printc("#################################################\n", 2);

  /* Consumer finalization */
  printc("#####################################\n", 2);
  printc("         End Process\n", 6);
  printc("PID:", 2);
  printc("\t\t\t2\n", 3);
  printc("Type:", 2);
  printc("\t\t\tMagic Number\n", 3);
  printc("Consumed Messages:", 2);
  printc("\t2\n", 3);
  printc("Waiting Time:", 2);
  printc("\t\t2s\n", 3);
  printc("Blocked Time:", 2);
  printc("\t\t2s\n", 3);
  printc("User time:", 2);
  printc("\t\t2s\n", 3);
  printc("#####################################\n", 2);

  /* Producer finalization */
  printc("#####################################\n", 2);
  printc("           End Process\n", 6);
  printc("PID:", 2);
  printc("\t\t\t2\n", 3);
  printc("Produced Messages:", 2);
  printc("\t2\n", 3);
  printc("Waiting Time:", 2);
  printc("\t\t2s\n", 3);
  printc("Blocked Time:", 2);
  printc("\t\t2s\n", 3);
  printc("#####################################\n", 2);

  /* Finalizer Message */
  printc("#############################################\n", 2);
  printc("                 End System\n", 6);
  printc("Total Messages:", 2);
  printc("\t\t\t\t2\n", 3);
  printc("Messages in Buffer:", 2);
  printc("\t\t\t2\n", 3);
  printc("Consumers:", 2);
  printc("\t\t\t\t2\n", 3);
  printc("Producers:", 2);
  printc("\t\t\t\t2\n", 3);
  printc("Consumers End by Magic Number:", 2);
  printc("\t\t2\n", 3);
  printc("Total Waiting Time:", 2);
  printc("\t\t\t2s\n", 3);
  printc("Total Blocked Time:", 2);
  printc("\t\t\t2s\n", 3);
  printc("Total User Time:", 2);
  printc("\t\t\t2s\n", 3);
  printc("#############################################\n", 2);

  return 0;


}
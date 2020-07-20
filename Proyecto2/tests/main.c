#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <libconfig.h>
#include <pthread.h>
#include <string.h>

const int X_SIZE = 46;
const int Y_SIZE = 24;

typedef struct
{
  int pid;
  int magic_number;
  int is_used;
  char date[50];
} message;

void load_map(int map[Y_SIZE][X_SIZE]);
void load_config(message *hola);
void *print_message_function(void *param);
int valdite_args(int argc, char *argv[], int *medium);
int is_number(char *text);
void hola(message *list, int *size);

config_t cfg, *cf;

int main(int argc, char *argv[])
{
  // int map[24][46];

  // load_map(map);

  // for (int i = 0; i < 24; ++i)
  // {
  //   for (int j = 0; j < 46; ++j)
  //   {
  //     printf("%d", map[i][j]);
  //     printf(" ");
  //   }
  //   printf("\n");
  // }
  // message hola;

  // const config_setting_t *retries;
  // const char *base = NULL;
  // int count, n, enabled;

  // cf = &cfg;
  // config_init(cf);

  // if (!config_read_file(cf, "ldap.cfg"))
  // {
  //   fprintf(stderr, "%s:%d - %s\n",
  //           config_error_file(cf),
  //           config_error_line(cf),
  //           config_error_text(cf));
  //   config_destroy(cf);
  //   return (EXIT_FAILURE);
  // }

  // hola.pid = 12;

  // load_config(&hola);

  // printf("%d\n", hola.pid);

  // config_destroy(cf);

  // pthread_t thread1;
  // int iret1;

  // iret1 = pthread_create(&thread1, NULL, &print_message_function, NULL);
  // if (iret1)
  // {
  //   fprintf(stderr, "Error - pthread_create() return code: %d\n", iret1);
  //   exit(EXIT_FAILURE);
  // }

  // printf("pthread_create() for thread 1 returns: %d\n", iret1);
  // pthread_join(thread1, NULL);
  // int hola = 0;
  // if (!valdite_args(argc, argv, &hola))
  // {
  //   return 1;
  // }

  // if (hola != 0)
  // {
  //   printf("Automatic Mode Selected\n");
  //   printf("%d\n", hola);
  // }
  // else
  // {
  //   printf("Manual Mode Selected\n");
  // }

  message list[1];

  int size = 0;

  list[0].pid = 5;

  printf("LIST PID = %d\n", list[0].pid);

  hola(list, &size);

  printf("LIST PID = %d\n", list[0].pid);
  printf("LIST SIZE = %d\n", size);

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

void load_config(message *hola)
{
  config_lookup_int(cf, "weight", &hola->pid);
}

void *print_message_function(void *param)
{
  printf("Hello World!\n");
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

void hola(message *list, int *size)
{
  list[*size].pid = 10;

  *size = *size + 1;
}
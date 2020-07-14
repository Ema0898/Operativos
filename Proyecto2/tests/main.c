#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <libconfig.h>

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

config_t cfg, *cf;

int main()
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
  message hola;

  const config_setting_t *retries;
  const char *base = NULL;
  int count, n, enabled;

  cf = &cfg;
  config_init(cf);

  if (!config_read_file(cf, "ldap.cfg"))
  {
    fprintf(stderr, "%s:%d - %s\n",
            config_error_file(cf),
            config_error_line(cf),
            config_error_text(cf));
    config_destroy(cf);
    return (EXIT_FAILURE);
  }

  hola.pid = 12;

  load_config(&hola);

  printf("%d\n", hola.pid);

  config_destroy(cf);
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
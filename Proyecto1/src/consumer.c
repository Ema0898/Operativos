#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <sys/sem.h>
#include <string.h>
#include <structs.h>
#include <shmem.h>
#include <semaphore.h>
#include <utilities.h>

double u_random();
int poisson(int lambda);
void read_msg(int index, struct sembuf operation, int id, message *memory);

int main(int argc, char *argv[])
{
  /* Argument validation */
  struct sembuf operation;

  char *buffer_name = argv[1];
  int lambda = atoi(argv[2]);

  if (argc != 3)
  {
    printf("Usage: ./consumer <buffer_name> <time_medium>\n");
    exit(0);
  }

  if (lambda == 0 || !is_number(argv[2]))
  {
    printf("Please insert a correct time medium size\n");
    exit(0);
  }

  /* Shared memory for global variables initializartion */
  key_t key_memory;
  int id_memory;
  message *memory = NULL;

  key_t key_global;
  int gv_shm_id;
  global_variables *memory2 = NULL;

  if (check_bin_dir())
  {
    key_global = ftok("../share_files/global", 33);
  }
  else
  {
    key_global = ftok("share_files/global", 33);
  }

  if (key_global == -1)
  {
    printf("Global Memory Key is Invalid\n");
    exit(0);
  }

  if (get_global(&gv_shm_id, key_global) == 0)
  {
    printf("Can't get global id\n");
    exit(0);
  }

  if (get_global_memory(&gv_shm_id, &memory2) == 0)
  {
    printf("Can't get global memory\n");
    exit(0);
  }

  /* Gets buffer size and increments producers counter */
  int buffer_size = memory2->size;
  memory2->consumers++;

  /* Shared memory for buffer initializartion */
  char *key_route;
  if (check_bin_dir())
  {
    key_route = concat("../share_files/", buffer_name);
  }
  else
  {
    key_route = concat("share_files/", buffer_name);
  }

  key_memory = ftok(key_route, 33);
  if (key_memory == -1)
  {
    printf("Shared Memory Key is Invalid\n");
    exit(0);
  }

  if (get_buffer(&id_memory, key_memory, 1024) == 0)
  {
    printf("Can't get buffer memory\n");
    exit(0);
  }

  if (get_buffer_memory(&id_memory, &memory) == 0)
  {
    printf("Can't get buffer memory\n");
    exit(0);
  }

  /* Create semaphores */
  int id_semaphore;
  if (check_bin_dir())
  {
    id_semaphore = init_semaphore("../share_files/sem", buffer_size);
  }
  else
  {
    id_semaphore = init_semaphore("share_files/sem", buffer_size);
  }

  operation.sem_flg = 0;

  int index;

  while (1)
  {
    int p = poisson(lambda);
    sleep(p);

    index = rand() % 1023;
    operation.sem_num = 0;
    operation.sem_op = -1;

    read_msg(0, operation, id_semaphore, memory);
  }

  return 0;
}

double u_random()
{ /* generates a 0 ~ Random number between 1 */
  double f;
  srand((unsigned)time(NULL));
  f = (float)(rand() % 100);
  /* Printf ("% fn", f );*/
  return f / 100;
}

/* Poisson Distribution */
int poisson(int lambda)
{
  int k = 0;
  double p = 1.0;
  double l = exp(-lambda);
  while (p >= l)
  {
    double u = u_random();
    p *= u;
    k++;
  }
  return k - 1;
}

/* Read from shared memory */
void read_msg(int index, struct sembuf operation, int id, message *memory)
{
  /* Down */
  semop(id, &operation, 1);

  /* Memory Read */
  printf("PID = %d\n", memory[index].pid);
  printf("Magic Number = %d\n", memory[index].magic_number);
  printf("Date = %s \n", memory[index].date);

  /* Up */
  operation.sem_op = 1;
  semop(id, &operation, 1);
}
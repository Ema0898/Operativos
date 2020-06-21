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

double ran_expo(double lambda);
void write_msg(int data1, int data2, char *data3, int index, struct sembuf operation, int id, message *memory);

int main(int argc, char *argv[])
{
  /* Argument validation */
  struct sembuf operation;

  char *buffer_name = argv[1];
  float seconds = atof(argv[2]);

  if (argc != 3)
  {
    printf("Usage: ./producer <buffer_name> <time_medium>\n");
    exit(0);
  }

  if (seconds == 0 || !is_float(argv[2]))
  {
    printf("Please insert a correct time medium size\n");
    exit(0);
  }

  srand((unsigned)time(NULL));

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
  memory2->producers++;

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

  if (get_buffer(&id_memory, key_memory, buffer_size) == 0)
  {
    printf("Can't get buffer id\n");
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
    float s = ran_expo(seconds);
    printf("%f \n", s);
    sleep(s);

    index = rand() % 1023;
    operation.sem_num = 0;
    operation.sem_op = -1;
    write_msg(1, 1, "Hola", 0, operation, id_semaphore, memory);
  }

  return 0;
}

/* Exponential Distribution */
double ran_expo(double lambda)
{
  double u;
  u = rand() / (RAND_MAX + 1.0);
  return -log(1 - u) / lambda;
}

/* Write on shared memory */
void write_msg(int data1, int data2, char *data3, int index, struct sembuf operation, int id, message *memory)
{
  /* Down */
  semop(id, &operation, 1);

  /* Memory Write */
  memory[index].pid = data1;
  memory[index].magic_number = data2;
  strcpy(memory[index].date, data3);

  /* Up */
  operation.sem_op = 1;
  semop(id, &operation, 1);
}

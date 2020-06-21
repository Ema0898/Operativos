#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <time.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <string.h>
#include <structs.h>
#include <shmem.h>
#include <semaphore.h>
#include <utilities.h>

double u_random();
int possion(int lambda);
void read_msg(int index, struct sembuf operation, int id, message *memory);

int main(int argc, char *argv[])
{
  //La vara de semaforos
  struct sembuf operation;

  char *buffer_name = argv[1];
  int lambda = atoi(argv[2]);

  if (argc != 3)
  {
    printf("Usage: ./consumer <buffer_name> <time_medium>\n");
    exit(0);
  }  

  //Memory
  key_t key_memory;
  int id_memory;
  message *memory = NULL;

  key_t key_global;
  int gv_shm_id;
  global_variables *memory2 = NULL;

  key_global = ftok("share_files/global", 33);
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

  int buffer_size = memory2->size;
  memory2->consumers++;

  char *key_route = concat("share_files/", buffer_name);

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
  int id_semaphore = init_semaphore("share_files/sem", buffer_size);

  operation.sem_flg = 0;

  //Bloques de memoria
  int index;

  while (1)
  {
    int p = possion(lambda);
    sleep(p);

    //Leer en memoria
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

int possion(int lambda) /* generates a random number with a Poisson distribution. Lamda is the average number */
{
  int k = 0;
  double p = 1.0;
  double l = exp(-lambda); /* it is defined as long double for precision, and exp (-Lambda) is a decimal near 0 */
  while (p >= l)
  {
    double u = u_random();
    p *= u;
    k++;
  }
  return k - 1;
}

void read_msg(int index, struct sembuf operation, int id, message *memory)
{
  // Down
  semop(id, &operation, 1);

  // Read write
  printf("PID = %d\n", memory[index].pid);
  printf("Magic Number = %d\n", memory[index].magic_number);
  printf("Date = %s \n", memory[index].date);

  // Up
  operation.sem_op = 1;
  semop(id, &operation, 1);
}
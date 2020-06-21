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

double ran_expo(double lambda);
void write_msg(int data1, int data2, char *data3, int index, struct sembuf operation, int id, message *memory);

int main(int argc, char *argv[])
{
  // Semaforos
  struct sembuf operation;

  char *buffer_name = argv[1];
  float seconds = atof(argv[2]);

  if (argc != 3)
  {
    printf("Cantidad de argumentos incorrecta\n");
    exit(0);
  }

  int id_semaphore = init_semaphore("share_files/sem");

  operation.sem_flg = 0;

  //Setea cuando se hace random (Setea semilla)
  srand((unsigned)time(NULL));

  // Memoria compartida
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

  char *key_route = concat("share_files/", buffer_name);
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

  //Bloques de memoria
  int index;

  while (1)
  {
    float s = ran_expo(seconds);
    printf("%f \n", s);
    sleep(s);

    //Escribir en memoria
    index = rand() % 1023;
    operation.sem_num = 0;
    operation.sem_op = -1;
    write_msg(1, 1, "Hola", 0, operation, id_semaphore, memory);
  }
}

double ran_expo(double lambda)
{
  double u;
  u = rand() / (RAND_MAX + 1.0);
  return -log(1 - u) / lambda;
}

void write_msg(int data1, int data2, char *data3, int index, struct sembuf operation, int id, message *memory)
{
  // Down
  semop(id, &operation, 1);

  // Memory write
  memory[index].pid = data1;
  memory[index].magic_number = data2;
  strcpy(memory[index].date, data3);

  // Up
  operation.sem_op = 1;
  semop(id, &operation, 1);
}

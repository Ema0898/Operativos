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
#include <sys/time.h>

double waiting_time;
double blocked_time;
struct timeval tic, toc, tic2, toc2;
double result;
int counter;
int memory_index = 0;

double ran_expo(double lambda);
void write_msg(int data1, int data2, char *data3, int index, struct sembuf operation, int id, message *memory);

int main(int argc, char *argv[])
{
  /* Argument validation */  
  
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

  /* Poner semaforo a variables globales */
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

  struct sembuf operation;
  operation.sem_flg = 0;
  int current_pid = getpid();

  while (1)
  {
    float s = ran_expo(seconds);
    waiting_time += s;
    printf("%f \n", s);
    sleep(s);    

    /* Down for producer semaphore */
    operation.sem_num = buffer_size + 1;
    operation.sem_op = -1;
    
    gettimeofday(&tic, NULL);
  
    semop(id_semaphore, &operation, 1);

    gettimeofday(&toc, NULL);
    result = (toc.tv_sec - tic.tv_sec);

    blocked_time += result;

    printf("Pasó down de semaforo productor\n");

    /* Get next index to write */
    //int index = get_index(0, buffer_size, memory);
    //printf("Write memory index %d\n", index);

    int index = get_index(0, buffer_size, memory, memory_index, id_semaphore);
    memory_index = index + 1;

    /* Write Operation */
    operation.sem_num = index;
    operation.sem_op = -1;

    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    char date[64];
    strftime(date, sizeof(date), "%c", tm);

    gettimeofday(&tic2, NULL);

    printf("Index: %d\n", index);
    semop(id_semaphore, &operation, 1);

    gettimeofday(&toc2, NULL);
    result = (toc2.tv_sec - tic2.tv_sec);

    blocked_time += result;

    printf("bloked time: %f\n", blocked_time);
  
    write_msg(current_pid, rand() % 6, date, index, operation, id_semaphore, memory);

    operation.sem_op = 1;
    semop(id_semaphore, &operation, 1);

    counter++;

    /* Up for consumer semaphore */
    printf("Productor UP 1\n");
    operation.sem_num = buffer_size + 2;
    operation.sem_op = 1;
    semop(id_semaphore, &operation, 1);
    printf("Productor UP 2\n");

    operation.sem_num = buffer_size;
    operation.sem_op = -1;
    semop(id_semaphore, &operation, 1);

    memory2->waiting_time += waiting_time;
    memory2->blocked_time += blocked_time;

    operation.sem_op = 1;
    semop(id_semaphore, &operation, 1);
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
  
  // semop(id, &operation, 1);

  /* Memory Write */
  memory[index].pid = data1;
  memory[index].magic_number = data2;
  memory[index].is_used = 1;
  strcpy(memory[index].date, data3);

  /* Up */
  //operation.sem_op = 1;
  //semop(id, &operation, 1);
}

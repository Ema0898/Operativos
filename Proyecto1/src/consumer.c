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
#include <print.h>

double waiting_time;
double blocked_time;
double user_time;
struct timeval tic, toc, tic2, toc2, tic3, toc3;
double result;
int counter;
int memory_index = 0;

double u_random();
int poisson(int lambda);
int read_msg(int index, struct sembuf operation, int id, message *memory, int pid_magic, global_variables *memory2, int buffer_size);

int main(int argc, char *argv[])
{
  /* Argument validation */
  char *buffer_name = argv[1];
  int lambda = atoi(argv[2]);
  int pid_magic = getpid() % 6;
  printf("Mi numero magico es: %d\n", pid_magic);

  if (argc != 4)
  {
    printf("Usage: ./consumer <buffer_name> <time_medium> <operation_mode (manual/automatic)>\n");
    exit(0);
  }

  if (lambda == 0 || !is_number(argv[2]))
  {
    printf("Please insert a correct time medium size\n");
    exit(0);
  }

  if (strcmp(argv[3], "manual") != 0 && strcmp(argv[3], "automatic") != 0)
  {
    printf("Please insert a correct operation mode\n");
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

  /* Shared memory for buffer initialization */
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

  /* Increments consumers counter */
  struct sembuf operation;

  operation.sem_num = buffer_size;
  operation.sem_op = -1;
  semop(id_semaphore, &operation, 1);

  memory2->consumers++;
  memory2->total_consumers++;

  operation.sem_op = 1;
  semop(id_semaphore, &operation, 1);

  int current_pid = getpid();

  while (1)
  {
    /* Check for manual mode flag */
    if (!strcmp(argv[3], "manual"))
    {
      printc("Press enter to consume message\n", 2);
      
      gettimeofday(&tic3, NULL);
      
      getchar();

      /* Get user time */
      gettimeofday(&toc3, NULL);
      result = (double)(toc3.tv_usec - tic3.tv_usec) / 1000000 + (double)(toc3.tv_sec - tic3.tv_sec);
      user_time += result;
    }

    int p = poisson(lambda);

    if (p <= 0 || p >= 10)
    {
      p = 5;
    }

    waiting_time += p;
    char waiting_print[20];
    sprintf(waiting_print, "Waiting time %d\n", p);
    printc(waiting_print, 1);
    sleep(p);    

    /* Down for consumer semaphore */
    operation.sem_num = buffer_size + 2;
    operation.sem_op = -1;
    
    /* Get blocked time consumer semaphore */
    gettimeofday(&tic, NULL);

    semop(id_semaphore, &operation, 1);

    gettimeofday(&toc, NULL);
    result = (double)(toc.tv_usec - tic.tv_usec) / 1000000 + (double)(toc.tv_sec - tic.tv_sec);

    blocked_time += result;

    /* Writes to global memory */
    operation.sem_num = buffer_size;
    operation.sem_op = -1;
    semop(id_semaphore, &operation, 1);

    /* Check for finish flag */
    if (memory2->kill)
    {
      /* Writes stadistics on global varibles */
      memory2->waiting_time += waiting_time;
      memory2->user_time += user_time;
      memory2->blocked_time += blocked_time;
      memory2->consumers--;
      operation.sem_op = 1;
      semop(id_semaphore, &operation, 1);

      print_consumer_end(current_pid, 1, counter, waiting_time, blocked_time, user_time);

      exit(0);
    }

    operation.sem_op = 1;
    semop(id_semaphore, &operation, 1);

    /* Get next index to read */
    int index = get_index(1, buffer_size, memory, memory_index, id_semaphore);
    memory_index = index + 1;

    /* Read Operation, Down to semaphore */
    operation.sem_num = index;
    operation.sem_op = -1;

    int msg_flag = 0;

    gettimeofday(&tic2, NULL);

    semop(id_semaphore, &operation, 1);

    gettimeofday(&toc2, NULL);
    result = (double)(toc2.tv_usec - tic2.tv_usec) / 1000000 + (double)(toc2.tv_sec - tic2.tv_sec);

    blocked_time += result;

    /* Read from shared memory */
    msg_flag = read_msg(index, operation, id_semaphore, memory, pid_magic, memory2, buffer_size);
    
    operation.sem_op = 1;
    semop(id_semaphore, &operation, 1);
    counter++;

    /* Up for producer semaphore */
    operation.sem_num = buffer_size + 1;
    operation.sem_op = 1;
    semop(id_semaphore, &operation, 1);

    /* Check for magic number flag */
    if (msg_flag)
    {
      /* Writes stadistics on global varibles */
      operation.sem_num = buffer_size;
      operation.sem_op = -1;
      semop(id_semaphore, &operation, 1);

      memory2->consumers--; 
      memory2->magic++;
      memory2->waiting_time += waiting_time;
      memory2->user_time += user_time;
      memory2->blocked_time += blocked_time;       

      operation.sem_op = 1;
      semop(id_semaphore, &operation, 1);

      print_consumer_end(getpid(), 0, counter, waiting_time, blocked_time, user_time);

      exit(0);
    }
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
int  read_msg(int index, struct sembuf operation, int id, message *memory, int pid_magic, global_variables *memory2, int buffer_size)
{
  /* Down */

  // semop(id, &operation, 1);

  /* Memory Read */
  memory[index].is_used = 0;

  operation.sem_num = buffer_size;
  operation.sem_op = -1;
  semop(id, &operation, 1);

  int consumers = memory2->consumers;
  int producers = memory2->producers;   
  
  operation.sem_op = 1;
  semop(id, &operation, 1);

  print_consumer_message(index, consumers, producers,  memory[index].date, memory[index].pid, memory[index].magic_number);

  /* Up */
  // operation.sem_op = 1;
  // semop(id, &operation, 1);

  if(pid_magic == memory[index].magic_number) {
    return 1;
  }

  return 0;
}

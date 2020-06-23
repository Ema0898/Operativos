#include <stdio.h>
#include <stdlib.h>
#include <sys/sem.h>
#include <utilities.h>
#include <structs.h>
#include <shmem.h>
#include <semaphore.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
  /* Argument Validation */
  if (argc != 3)
  {
    printf("Usage: ./finalizer <buffer_size> <buffer_name>\n");
    exit(0);
  }

  int buffer_size = atoi(argv[1]);
  char *buffer_name = argv[2];

  if (buffer_size == 0 || !is_number(argv[1]))
  {
    printf("Please insert a correct buffer size\n");
    exit(0);
  }

  /* Shared Memory Buffer Initialization */
  char *key_route;
  if (check_bin_dir())
  {
    key_route = concat("../share_files/", buffer_name);
  }
  else
  {
    key_route = concat("share_files/", buffer_name);
  }

  key_t key;
  int id_memory;

  key = ftok(key_route, 33);
  if (key == -1)
  {
    printf("Shared Memory Key is Invalid\n");
    exit(0);
  }

  if (get_buffer(&id_memory, key, buffer_size) == 0)
  {
    printf("Shared Memory Key is Invalid\n");
    exit(0);
  }

  /* Shared Memory for Global Variables Initialization */
  int gv_shm_id;
  key_t key_global;
  global_variables *memory2;  

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
    printf("Can't get global memory to finish it\n");
    exit(0);
  }

  if (get_global_memory(&gv_shm_id, &memory2) == 0)
  {
    printf("Can't get buffer memory\n");
    exit(0);
  }

  /* Get Semaphore */
  int id_semaphore;
  if (check_bin_dir())
  {
    id_semaphore = init_semaphore("../share_files/sem", buffer_size);
  }
  else
  {
    id_semaphore = init_semaphore("share_files/sem", buffer_size);
  }

  printf("Waiting Time: %f\n", memory2->waiting_time);
  printf("User Time: %f\n", memory2->user_time);
  printf("Blocked Time: %f\n", memory2->blocked_time);

  /* Clears semaphores and shared memory for buffer and global variables */

  shmctl(id_memory, IPC_RMID, (struct shmid_ds *)NULL);

  shmctl(gv_shm_id, IPC_RMID, (struct shmid_ds *)NULL);

  semctl(id_semaphore, 0, IPC_RMID, NULL);

  /* Deletes files for shared memory and semaphore */

  if (check_bin_dir())
  {
    system("rm -rf ../share_files");
  }
  else
  {
    system("rm -rf share_files");
  }

  return 0;
}
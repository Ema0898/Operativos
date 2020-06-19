#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdlib.h>
#include <stdio.h>

#if defined(__GNU_LIBRARY__) && !defined(_SEM_SEMUN_UNDEFINED)
// The union is already defined in sys/sem.h
#else
union semun {
  int val;
  struct semid_ds *buf;
  unsigned short int *array;
  struct seminfo *__buf;
};
#endif

int main()
{
  key_t key;
  int id_semaphore;
  struct sembuf operation;
  union semun arg;

  key = ftok("/bin/ls", 33);

  if (key == -1)
  {
    printf("Can't get semaphore key\n");
    exit(0);
  }

  id_semaphore = semget(key, 10, 0600 | IPC_CREAT);

  if (id_semaphore == -1)
  {
    printf("Can't create semaphore\n");
    exit(0);
  }

  arg.val = 0;
  semctl(id_semaphore, 0, SETVAL, &arg);

  operation.sem_num = 0;
  operation.sem_op = -1;
  operation.sem_flg = 0;

  int i = 0;

  while (1)
  {
    printf("%d Waiting for Semaphore\n", i);

    semop(id_semaphore, &operation, 1);
    /* Write on shared memory */

    printf("%d Exit from Semaphore\n", i);

    i++;
  }

  return 0;
}
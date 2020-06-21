#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

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

  id_semaphore = semget(key, 10, 0600);

  if (id_semaphore == -1)
  {
    printf("Can't create semaphore\n");
    exit(0);
  }

  operation.sem_num = 0;
  operation.sem_op = 1;
  operation.sem_flg = 0;

  for (int i = 0; i < 10; ++i)
  {
    printf("Semaphore Up \n");

    semop(id_semaphore, &operation, 1);
    sleep(1);
  }

  semctl(id_semaphore, 0, IPC_RMID, NULL);
}
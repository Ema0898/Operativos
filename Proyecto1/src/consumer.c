#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <time.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <structs.h>

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

double U_Random (){/* generates a 0 ~ Random number between 1 */
  double f;
  srand((unsigned)time(NULL));
  f = (float) (rand () % 100 );
  /* Printf ("% fn", f );*/
  return f / 100;
}
 
int possion ()/* generates a random number with a Poisson distribution. Lamda is the average number */
{
  int lambda = 1, k = 0;
  double p = 1.0;
  double l = exp(-lambda);/* it is defined as long double for precision, and exp (-Lambda) is a decimal near 0 */  
  while (p >= l) {
    double u = U_Random ();
    p *= u;
    k++;
  }
  return k-1;
}

//Read
void read_msg(int index, struct sembuf operation, int id, message *memory){
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

int main(int argc, char* argv[]){
  //La vara de semaforos
  key_t key;
  int id_semaphore;
  struct sembuf operation;
  union semun arg;

  char* buffer_name = argv[1];
  float seconds = atof(argv[2]);

  if(argc != 3){
    printf("Cantidad de argumentos incorrecta\n");
    exit(0);
  }

  key = ftok("/bin/cat", 33);

  if (key == -1)
  {
    printf("Can't get semaphore key\n");
    exit(0);
  }

  id_semaphore = semget(key, 1024, 0600);

  if (id_semaphore == -1)
  {
    printf("Can't create semaphore\n");
    exit(0);
  }

  arg.val = 0;
  semctl(id_semaphore, 0, SETVAL, &arg);

  operation.sem_flg = 0;
  
  //Memory
  key_t key_memory;
  int id_memory;
  message *memory = NULL;

  key_memory = ftok(buffer_name, 33);
  if (key_memory == -1)
  {
    printf("Shared Memory Key is Invalid\n");
    exit(0);
  }

  id_memory = shmget(key_memory, sizeof(message) * 1024, 0777);
  if (id_memory == -1)
  {
    printf("Shared Memory Id is Invalid\n");
    exit(0);
  }

  memory = (message *)shmat(id_memory, (char *)0, 0);
  if (memory == NULL)
  {
    printf("Can't get shared memory\n");
    exit(0);
  }

  //Bloques de memoria
  int index;

  while(1) {
    int p = possion();
    sleep(p);

    //Leer en memoria 
    index = rand() % 1023;
    operation.sem_num = 0;
    operation.sem_op = -1;

    read_msg(0, operation, id_semaphore, memory);    
  }

  return 0;
}
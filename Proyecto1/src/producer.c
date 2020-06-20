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

double ran_expo(double lambda) {
  double u;
  u = rand() / (RAND_MAX + 1.0);
  return -log(1- u) / lambda;
}  

//Write message
void write_msg(int data1, int data2, char* data3, int index, struct sembuf operation,int id, message *memory) {
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

int main(int argc, char* argv[]) {
  // Semaforos
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
  
  //Setea cuando se hace random (Setea semilla)
  srand((unsigned)time(NULL));

  printf("PADRE: Soy el proceso padre y mi pid es: %d\n", getpid());

  // Memoria compartida
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

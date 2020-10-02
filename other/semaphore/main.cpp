#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <iostream>

sem_t mutex;

void* thread(void* arg)
{
  sem_wait(&mutex);
  printf("\nEntered..\n");

  sleep(4);

  printf("\nJust Exiting...\n");
  sem_post(&mutex);
}

int main(int argc ,char **argv)
{
  std::cout << "Semaphore Test" << std::endl;

  // int sem_init(sem_t *sem, int pshared, unsigned int value);
  // "pshared" indicates whether this semaphore is to be shared between the threads of a process,
  // or between processes.
  // "value" specifies the semaphore's value. sem_wait() decreases the value if it is greater than
  // zero, or it blocks.
  sem_init(&mutex, 0, 1);
  pthread_t t1,t2;

  pthread_create(&t1,NULL,thread,NULL);
  sleep(2);
  pthread_create(&t2,NULL,thread,NULL);

  pthread_join(t1,NULL);
  pthread_join(t2,NULL);
  sem_destroy(&mutex);

  return 0;
}

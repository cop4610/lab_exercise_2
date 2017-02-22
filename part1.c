#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "testprint.h"

typedef enum {true, false} bool;


int SharedVariable = 0;
pthread_barrier_t barrier;
pthread_barrier_t syscall_barrier;
pthread_mutex_t mutex;

/**
* Thread-based action
*/
void SimpleThread(int thredID)
{
    #ifdef SYSCALL_SYNC
      int barrier_wait_code2 = pthread_barrier_wait(&syscall_barrier);
        if (barrier_wait_code2 &&
            barrier_wait_code2 != PTHREAD_BARRIER_SERIAL_THREAD) {
            printf("Could not wait on barrier\n");
            exit(-1);
        }
    #endif
  int num, val;

  for (num = 0; num < 20; num++) {
    if (random() > RAND_MAX / 2) {
      usleep(10);
    }

    #ifdef PTHREAD_SYNC
      pthread_mutex_lock(&mutex);
    #endif

    val = SharedVariable;
    printf("*** thread %d sees value %d\n", thredID, val);

    SharedVariable = val + 1;

    #ifdef PTHREAD_SYNC
      pthread_mutex_unlock(&mutex);
    #endif
  }

  #ifdef PTHREAD_SYNC
    int barrier_wait_code = pthread_barrier_wait(&barrier);
    if (barrier_wait_code &&
        barrier_wait_code != PTHREAD_BARRIER_SERIAL_THREAD) {
        printf("Could not wait on barrier\n");
        exit(-1);
    }
  #endif

  #ifdef SYSCALL_SYNC
    barrier_wait_code2 = pthread_barrier_wait(&syscall_barrier);
      if (barrier_wait_code2 &&
          barrier_wait_code2 != PTHREAD_BARRIER_SERIAL_THREAD) {
          printf("Could not wait on barrier\n");
          exit(-1);
      }
  #endif

  val = SharedVariable;
  printf("Thread %d sees final value %d\n", thredID, val);
}

/**
* Checks if the string is a number
* @return {bool} true if number, false if not
*/
bool isNumber(char* input)
{
  int index = 0;

  while (input[index] != '\0') {
    switch (input[index]) {
      case '0':
      case '1':
      case '2':
      case '3':
      case '4':
      case '5':
      case '6':
      case '7':
      case '8':
      case '9':
        index++;
        continue;
      default:
        return false;
    }
  }

  // Empty String - return false
  if (index == 0) {
    return false;
  }
  return true;
}

/**
* Main program
*/
int main(int argc, char* argv[])
{

  int thread_count, i;

  void* ThreadedAction(void* parameter) {
    SimpleThread((int) parameter);
    return (void *) 0;
  }

  if (argc != 2 || isNumber(argv[1]) == false) {
    printf("Please provide a valid integer as a parameter.\n");
    return -1;
  }

  thread_count = atoi(argv[1]);
  if (thread_count <= 0) {
    printf("Please provide an integer greater than 0.\n");
    return -1;
  }

  pthread_t threads[thread_count];

  if (pthread_barrier_init(&barrier, NULL, thread_count)) {
    printf("Error initializing pthread barrier!\n");
    return -1;
  }
  //syscall barrier will make all n threads wait for the system call to happen to ensure it happens after they start but before they finish.
  if (pthread_barrier_init(&syscall_barrier, NULL, thread_count+1)) {
    printf("Error initializing syscall barrier!\n");
    return -1;
  }
  if (pthread_mutex_init(&mutex, NULL)) {
    printf("Error initializing mutex!\n");
    return -1;
  }

  for (i = 0; i < thread_count; i++) {
    int error_code = pthread_create(&threads[i], NULL, ThreadedAction, (void *) i);
    if (error_code) {
      printf("Error initializing thread!\n");
      return -1;
    }

    printf("Created Thread %d\n", i);
  }
  #ifdef SYSCALL_SYNC
      int barrier_wait_code = pthread_barrier_wait(&syscall_barrier);
      if (barrier_wait_code &&
          barrier_wait_code != PTHREAD_BARRIER_SERIAL_THREAD) {
          printf("Could not wait on barrier\n");
          exit(-1);
      }
    printf("%ld\n", syscall(__NR_fajet_mizrahi_paragas));

     barrier_wait_code = pthread_barrier_wait(&syscall_barrier);
    if (barrier_wait_code &&
        barrier_wait_code != PTHREAD_BARRIER_SERIAL_THREAD) {
        printf("Could not wait on barrier\n");
        exit(-1);
    }
  #endif


  pthread_exit(NULL);
  return 0;

}

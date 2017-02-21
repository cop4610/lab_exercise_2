#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>

typedef enum {true, false} bool;

sem_t room_lock;
pthread_mutex_t chosen_reporter_lock;
pthread_barrier_t start_response_barrier;
pthread_barrier_t end_response_barrier;

pthread_t* reporter_list;
pthread_t speaker;

int chosen_reporter_id;

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

void AnswerStart()
{
  printf("Speaker starts to answer question for reporter %d.\n",
         chosen_reporter_id);
}

void AnswerDone()
{
  printf("Speaker is done with answer for reporter %d.\n",
         chosen_reporter_id);
}

void EnterConferenceRoom(int reporter_id)
{
  printf("Reporter %d enters the conference room.\n",
         reporter_id);
}

void LeaveConferenceRoom(int reporter_id)
{
  printf("Reporter %d leaves the conference room.\n",
         reporter_id);
}

void QuestionStart()
{
  printf("Reporter %d asks a question.\n",
         chosen_reporter_id);
}

void QuestionDone()
{
  printf("Reporter %d is satisfied.\n",
         chosen_reporter_id);
}

void Speaker()
{
  void* SpeakerAction(void* parameter) {
    while (1) {
      pthread_barrier_wait(&start_response_barrier);
      AnswerStart();
      AnswerDone();
      pthread_barrier_wait(&end_response_barrier);
    }

    return (void *) NULL;
  }

  pthread_create(&speaker, NULL, SpeakerAction, (void *) NULL);
}

void Reporter(int id)
{
  // Reporter's Sequence of Actions
  void* ReporterAction(void* parameter) {
    int reporter_id = (int) parameter;
    int questions_count = (reporter_id % 4) + 2;
    int i;

    // Less room space as we fit in one more reporter
    //  Decrement Counting Semaphore
    sem_wait(&room_lock);
    EnterConferenceRoom(reporter_id);

    for (i = 0; i < questions_count; i++) {
      if (random() > RAND_MAX / 2) {
        usleep(10);
      }

      // Acquire right to ask a question
      pthread_mutex_lock(&chosen_reporter_lock);

      // QuestionStart() ->
      // AnswerStart() ->
      // AnswerDone() ->
      // QuestionDone()
      chosen_reporter_id = reporter_id;
      QuestionStart();
      pthread_barrier_wait(&start_response_barrier);
      pthread_barrier_wait(&end_response_barrier);
      QuestionDone();

      // Release right to ask a question
      pthread_mutex_unlock(&chosen_reporter_lock);
    }

    // More room space as a reporter left
    //  Increment Counting Semaphore
    LeaveConferenceRoom(reporter_id);
    sem_post(&room_lock);

    return (void *) NULL;
  }

  pthread_create(&reporter_list[id], NULL, ReporterAction, (void *) id);
}

int main(int argc, char* argv[])
{

  int i;

  // Validate that inputs are integers
  if (argc !=3 ||
      isNumber(argv[1]) == false ||
      isNumber(argv[2]) == false) {
    printf("Please provide valid input - parameters must be integers.\n");
    return -1;
  }

  // Validate that inputs are greater than 0
  int reporters_count = atoi(argv[1]);
  int room_limit = atoi(argv[2]);
  if (reporters_count <= 0 || room_limit <= 0) {
    printf(
      "There must be at least 1 reporter and a room limit of at least 1.\n");
    return -1;
  }

  // Bootstrap setup
  reporter_list = malloc(reporters_count * sizeof(pthread_t));
  sem_init(&room_lock, 0, room_limit);
  pthread_mutex_init(&chosen_reporter_lock, NULL);
  pthread_barrier_init(&start_response_barrier, NULL, 2);
  pthread_barrier_init(&end_response_barrier, NULL, 2);

  // Start up a Speaker and Reporters
  Speaker();
  for (i = 0; i < reporters_count; i++) {
    Reporter(i);
  }

  // Teardown cleanup
  pthread_exit(NULL);
  free(reporter_list);
  return 0;

}

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h> 

#define number_philo 5

pthread_mutex_t cutlery[number_philo];
pthread_t phils[number_philo];
int locked;
int count;
pthread_t check;

void* philosopher (void* number) {
  int my_num = *((int*)number);

  while (1) {
    printf("\n Philosopher %d is thinking.\n",my_num);
    pthread_mutex_lock (&cutlery[my_num]);
    locked++;
    void* detect_deadlock();
    printf("\nPhilosopher %d has left cutlery. \n",my_num);
    sleep(1);
    pthread_mutex_lock (&cutlery[(my_num + 1) %  number_philo]);
    printf("\nPhilosopher %d has right cutlery. \n",my_num);

    printf("\n Philosopher %d eating.\n", my_num);
    sleep(5);

    printf("\n Philosopher %d done.\n", my_num);
    count++;

    pthread_mutex_unlock (&cutlery[(my_num + 1) % number_philo]);

    pthread_mutex_unlock (&cutlery[my_num]);
    locked--;
    printf("\nPhilosopher %d no longer has cutlery.\n", my_num);
  }

  return NULL;
}

void* detect_deadlock() {
  srand(time(NULL));
  while (1) {
    if (locked == 5) {
      int c = rand() % 5;
      printf("Deadlock detected, dropping Chopstick %d.", c);
      pthread_mutex_unlock (&cutlery[c]);
    }
    return NULL;
  }
}

int main () {
  int i;
  int phil_num[number_philo];

  void* return_val;

  for (i = 0; i < number_philo; i++) {
    pthread_mutex_init (&cutlery[i], NULL);
  }

  for (i = 0; i < number_philo; i++) {
    phil_num[i] = i;
    pthread_create (&phils[i], NULL, philosopher, &phil_num[i]);
    pthread_create (&check[i], NULL, detect_deadlock, NULL);
  }

  for (i = 0; i < number_philo; i++) {
    pthread_join (phils[i], &return_val);
    pthread_join (check, &return_val);
  }

  return 0;

}

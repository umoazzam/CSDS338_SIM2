#include <pthread.h>
#include <stdio.h>
#include <stdbool.h> 

#define number_philo 5

pthread_mutex_t cutlery[number_philo];
pthread_t phils[number_philo];
bool locked[number_philo];


void* philosopher (void* number) {
  int my_num = *((int*)number);

  while (1) {
    printf("\n Philosopher %d is thinking.\n",my_num);

    pthread_mutex_lock (&cutlery[my_num]);
    printf("\nPhilosopher %d has left cutlery. \n",my_num);
    sleep(10);
    pthread_mutex_lock (&cutlery[(my_num + 1) %  number_philo]);
    printf("\nPhilosopher %d has right cutlery. \n",my_num);

    printf("\n Philosopher %d eating.\n", my_num);

    printf("\n Philosopher %d done.\n", my_num);

    pthread_mutex_unlock (&cutlery[(my_num + 1) % number_philo]);

    pthread_mutex_unlock (&cutlery[my_num]);
    printf("\nPhilosopher %d no longer has cutlery.\n", my_num);
  }

  return NULL;
}

void* detect_deadlock() {
  int locks = 0;
  for (int i = 0; i < number_philo; i++) {
    if (locked[number_philo] == 1) {
      locks++;
    }
  }

  sleep(5);

  if (locks == number_philo) {
    pthread_cancel(phils[number_philo]);
  }

  return 0;
}

int main () {

  int i;
  int phil_num[number_philo];

  pthread_t phils[number_philo];

  void* return_val;


  for (i = 0; i < number_philo; i++) {
    pthread_mutex_init (&cutlery[i], NULL);
  }

  for (i = 0; i < number_philo; i++) {
    phil_num[i] = i;
    pthread_create (&phils[i], NULL, philosopher, &phil_num[i]);
    pthread_create (&check, NULL, detect_deadlock, NULL);
  }

  for (i = 0; i < number_philo; i++) {
    pthread_join (phils[i], &return_val);
  }

  return 0;

}

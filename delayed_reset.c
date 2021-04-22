#include <pthread.h>
#include <stdio.h>

#define number_philo 5

pthread_mutex_t cutlery[number_philo];


void* philosopher (void* number) {
  int my_num = *((int*)number);

  while (1) {
    printf("\n Philosopher %d is thinking.\n",my_num);

    pthread_mutex_lock (&cutlery[my_num]);
    printf("\nPhilosopher %d has left cutlery. \n",my_num);
    sleep(3);
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
  }

  for (i = 0; i < number_philo; i++) {
    pthread_join (phils[i], &return_val);
  }

  return 0;

}

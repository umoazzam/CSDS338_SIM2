#include <pthread.h>
#include <stdio.h>
#include <stdbool.h> 
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

#define number_philo 5

pthread_mutex_t cutlery[number_philo];
pthread_t phils[number_philo];
bool locked[number_philo];
int drop_prob = 0.2;
bool isEating[number_philo];
bool hasEaten[number_philo];
double runtime;


double rand_value()
{
    return (double)rand() / RAND_MAX;
}

void* philosopher(void* number) {
    int my_num = *((int*)number);
    srand(time(NULL));

    while (!hasEaten[my_num]) {
        printf("\n Philosopher %d is thinking.\n", my_num);

        pthread_mutex_lock(&cutlery[my_num]);
        printf("\nPhilosopher %d has left cutlery. \n", my_num);
        sleep(10);
        //if(rand_value() <= drop_prob) {
            pthread_mutex_unlock(&cutlery[my_num]);
            printf("\nPhilosopher %d has dropped left cutlery. \n", my_num);
       // }
        pthread_mutex_lock(&cutlery[(my_num + 1) % number_philo]);
        printf("\nPhilosopher %d has right cutlery. \n", my_num);

        printf("\n Philosopher %d eating.\n", my_num);
        isEating[my_num] = true;
        sleep(10);
        printf("\n Philosopher %d done.\n", my_num);
        isEating[my_num] = false;
        hasEaten[my_num] = true;

        pthread_mutex_unlock(&cutlery[(my_num + 1) % number_philo]);
        pthread_mutex_unlock(&cutlery[my_num]);
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

int main() {

    int i;
    int phil_num[number_philo];
    clock_t start, end;
    double runtime;

    pthread_t phils[number_philo];
    pthread_t check;

    for (int i = 0; i < number_philo; i++) {
        isEating[i] = false;
    }

    for (int i = 0; i < number_philo; i++) {
        hasEaten[i] = false;
    }

    void* return_val;

    start = clock();
    for (i = 0; i < number_philo; i++) {
        pthread_mutex_init(&cutlery[i], NULL);
    }

    for (i = 0; i < number_philo; i++) {
        phil_num[i] = i;
        pthread_create(&phils[i], NULL, philosopher, &phil_num[i]);
        pthread_create(&check, NULL, detect_deadlock, NULL);
    }

    for (i = 0; i < number_philo; i++) {
        pthread_join(phils[i], &return_val);
    }
    end = clock();
    runtime = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("\nIt took %f seconds for all the philosophers to eat.\n", runtime);
    return 0;

}

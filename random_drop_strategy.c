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
bool isEating[number_philo];
bool hasEaten[number_philo];
int eatNum[number_philo];
double runtime;
int value;
clock_t start;

void* philosopher(void* number) {
    int my_num = *((int*)number);
    int count = 0;
    restart:;
    for (int i = 0; i < number_philo; i++) {
        count += eatNum[i];
    }
    while (!hasEaten[my_num]) {

        printf("\n Philosopher %d is thinking.\n", my_num);

        pthread_mutex_lock(&cutlery[my_num]);
        printf("\nPhilosopher %d has left cutlery. \n", my_num);
        sleep(5);
        if (value == my_num && !(count >= 4)) {
            pthread_mutex_unlock(&cutlery[my_num]);
            printf("\nPhilosopher %d has dropped left cutlery. \n", my_num);
            goto restart;
        }
        pthread_mutex_lock(&cutlery[(my_num + 1) % number_philo]);
        printf("\nPhilosopher %d has right cutlery. \n", my_num);

        printf("\n Philosopher %d eating.\n", my_num);
        isEating[my_num] = true;
        sleep(5);
        printf("\n Philosopher %d done.\n", my_num);
        isEating[my_num] = false;
        hasEaten[my_num] = true;
        eatNum[my_num] = 1;

        pthread_mutex_unlock(&cutlery[(my_num + 1) % number_philo]);
        pthread_mutex_unlock(&cutlery[my_num]);
        printf("\nPhilosopher %d no longer has cutlery.\n", my_num);

        if (count >= 4) {
            clock_t end = clock();
            double runtime = ((double)(end - start)) / CLOCKS_PER_SEC;
            printf("\nIt took %f seconds for all the philosophers to eat.\n", runtime);
        }
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

    srand(time(NULL));
    value = rand() % 5;

    int i;
    int phil_num[number_philo];

    pthread_t phils[number_philo];
    pthread_t check;

    for (int i = 0; i < number_philo; i++) {
        isEating[i] = false;
        hasEaten[i] = false;
        eatNum[i] = 0;
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
    
    return 0;

}

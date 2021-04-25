#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

/*can change number of philosophers here, code should work for any number*/
#define NUMPHIL 5

/*methods*/
void *philosopher(void *);
void think(int);
void pickUp(int);
void eat(int);
void dropStick(int);

/*thread and mutex stuff*/
pthread_mutex_t chopsticks[NUMPHIL];
pthread_t philosophers[NUMPHIL];
pthread_attr_t attributes[NUMPHIL];

/*used to make implementation of allowing phil 3 to reach across table easier*/
/*index shows 1 when the chopstick at that index is being used, 0 when not*/
int useArr[NUMPHIL];
/*stores the "left" index of the chopstick for phil3, doesn't have t be on his left*/
/*used for dropStick method*/
int phil3left;
/*stores the "right" index of the chopstick for phil3, doesn't have t be on his right*/
/*used for dropStick method*/
int phil3right;

/*stores 1 if phil has eaten, 0 if not*/
int hasEaten[NUMPHIL];
/*if all have eaten stores 1, if not 0*/
int allEat = 0;



int main() {
	int i;
  /* setting up the array for the variation part of the problem*/
  for(i = 0; i < NUMPHIL; ++i){
    useArr[i] = 0;
    hasEaten[i] = 0;
  }

	srand(time(NULL));

	for (i = 0; i < NUMPHIL; ++i) {
		pthread_mutex_init(&chopsticks[i], NULL);
	}

	for (i = 0; i < NUMPHIL; ++i) {
		pthread_attr_init(&attributes[i]);
	}

	for (i = 0; i < NUMPHIL; ++i) {
		pthread_create(&philosophers[i], &attributes[i], philosopher, (void *)(i));
	}

	for (i = 0; i < NUMPHIL; ++i) {
		pthread_join(philosophers[i], NULL);
	}
	return 0;
}

void *philosopher(void *philNum) {

	while (allEat != 1) {
		think(philNum);
		pickUp(philNum);
		eat(philNum);
		dropStick(philNum);
	}
  
}

/*all philosophers eat for a max of 3 seconds, generated randomly*/
void think(int philNum) {
	int thinkTime = 5;
	printf("philosopher %d thinking for %d seconds\n", philNum, thinkTime);
	sleep(thinkTime);
}

void pickUp(int philNum) {
  int right = (philNum + 1) % NUMPHIL;
  int left = (philNum + NUMPHIL) % NUMPHIL;
  /*stores how many chopsticks phil3 has picked up so he doesn't pick up more than two*/
  int num3picked;
  
  /* index 2 refers to third philosopher*/
  if (philNum == 2) {
    num3picked = 0;
    while (num3picked < 2) { /*this sets phil2's "left" chopstick*/
      if(num3picked == 0){
        int j;
        while (useArr[j] != 0) {
          j = ((j + 1) % NUMPHIL);
        }
        printf("philosopher %d is trying to pick up chopstick %d\n", philNum, j);
        pthread_mutex_lock(&chopsticks[j]);
        printf("philosopher %d picked up chopstick %d\n", philNum, j);
        useArr[j] = 1;
        phil3left = j;
        num3picked = num3picked + 1;
      }
      else{ /*this sets phil2's "left" chopstick*/
        int j;
        while (useArr[j] != 0) {
          j = ((j + 1) % NUMPHIL);
        }
        printf("philosopher %d is trying to pick up chopstick %d\n", philNum, j);
        pthread_mutex_lock(&chopsticks[j]);
        printf("philosopher %d picked up chopstick %d\n", philNum, j);
        useArr[j] = 1;
        phil3right = j;
        num3picked = num3picked + 1;
      }
    }
  }
  else if (philNum & 1) { /*odd numbered philosophers pick up right chopstick first*/
  	printf("philosopher %d is trying to pick up chopstick %d\n", philNum, right);
  	pthread_mutex_lock(&chopsticks[right]);
    useArr[right] = 1;
	 	printf("philosopher %d picked up chopstick %d\n", philNum, right);
	 	printf("philosopher %d is trying to pick up chopstick %d\n", philNum, left);
	 	pthread_mutex_lock(&chopsticks[left]);
    useArr[left] = 1;
	 	printf("philosopher %d picked up chopstick %d\n", philNum, left);
	}
	else { /*even numbered chopsticks pick up left chopstick first*/
	 	printf("philosopher %d is trying to pick up chopstick %d\n", philNum, left);
	 	pthread_mutex_lock(&chopsticks[left]);
    useArr[left] = 1;
  	printf("philosopher %d picked up chopstick %d\n", philNum, left);
  	printf("philosopher %d is trying to pick up chopstick %d\n", philNum, right);
  	pthread_mutex_lock(&chopsticks[right]);
    useArr[right] = 1;
  	printf("philosopher %d picked up chopstick %d\n", philNum, right);
  }
}

/*all philosophers eat for a max of 3 seconds, generated randomly*/
void eat(int philNum) {
  hasEaten[philNum] = 1;
	int eatTime = 5;
	printf("philosopher %d will eat for %d seconds\n", philNum, eatTime);
	sleep(eatTime);
  int x = 0;

  while(hasEaten[x] != 0){
    x = x + 1;
    if (x == 5){
      allEat = 1;
      break;
    }
  }
}

void dropStick(int philNum) {
	printf("philosopher %d put down their chopsticks\n", philNum);

  if(philNum == 2){
    pthread_mutex_unlock(&chopsticks[phil3left]);
    useArr[phil3left] = 0;
    pthread_mutex_unlock(&chopsticks[phil3right]);
    useArr[phil3right] = 0;
  }
  else{
	pthread_mutex_unlock(&chopsticks[(philNum + 1) % NUMPHIL]);
  useArr[(philNum + 1) % NUMPHIL] = 0;
	pthread_mutex_unlock(&chopsticks[(philNum + NUMPHIL) % NUMPHIL]);
  useArr[(philNum + NUMPHIL) % NUMPHIL] = 0;
  }
}

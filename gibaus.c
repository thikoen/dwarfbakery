#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
#include <time.h>
#include <pthread.h>

/* Warten von 1 bis 10000 Millisekunden (10 Sekunden) */
void msleep(unsigned int ms)
{
	//Wating function
	struct timespec ts;

	assert(ms > 0);
	assert(ms <= 10000);

	ts.tv_sec = ms / 1000;
	ts.tv_nsec = (ms * 1000000) % 1000000000;
	nanosleep(&ts, NULL);
}

int getRandomInt(int from, int to)
{
	//Random function
	int result;
	result=to-from+1;
	result=from+(int)(result*rand()/(RAND_MAX+1.0));
	return result;
}

void threadFunction(char uniqueChar){
    
	//Function for thread work
	int printTimes=5, i, j, sleepTimeFrom=100, sleepTimeTo=500;
    
	for(i=0;i<printTimes;i++){
		//Print outs
		for(j=0;j<10;j++){
			printf("%c",uniqueChar);
			//Need to flush out every time
			fflush(stdout);
			//Sleep
			msleep(getRandomInt(sleepTimeFrom,sleepTimeTo));
		}
		printf("\n");
	}
}

int main(int argc, char *argv[])
{
	printf("gibaus.c\n");
    
	pthread_t threadOne, threadTwo, threadThree, threadFour, threadFive, threadSix;
    
	//Creating several threads, checking for errors
	if(pthread_create(&threadOne, NULL, (void*)&threadFunction, (void*)'A')!=0){
		printf("ERROR creating thread!");
		return EXIT_FAILURE;
	}
	if(pthread_create(&threadTwo, NULL, (void*)&threadFunction, (void*)'B')!=0){
		printf("ERROR creating thread!");
		return EXIT_FAILURE;
	}
	if(pthread_create(&threadThree, NULL, (void*)&threadFunction, (void*)'C')!=0){
		printf("ERROR creating thread!");
		return EXIT_FAILURE;
	}
	if(pthread_create(&threadFour, NULL, (void*)&threadFunction, (void*)'D')!=0){
		printf("ERROR creating thread!");
		return EXIT_FAILURE;
	}
	if(pthread_create(&threadFive, NULL, (void*)&threadFunction, (void*)'E')!=0){
		printf("ERROR creating thread!");
		return EXIT_FAILURE;
	}
	if(pthread_create(&threadSix, NULL, (void*)&threadFunction, (void*)'F')!=0){
		printf("ERROR creating thread!");
		return EXIT_FAILURE;
	}
    
	//Waiting for running threads
	pthread_join(threadOne, NULL);
	pthread_join(threadTwo, NULL);
	pthread_join(threadThree, NULL);
	pthread_join(threadFour, NULL);
	pthread_join(threadFive, NULL);
	pthread_join(threadSix, NULL);
    
	return EXIT_SUCCESS;
}

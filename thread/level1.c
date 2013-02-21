/******************************************************************************
 * Simple C thread application
 *
 * Compile command: gcc level1.c -o main -lpthread
 *
 * ihsan Kehribar - 2013
 *****************************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

void *threadFunction(void *arg)
{
	int threadNumber = (int)arg;

	while(1)
	{
		printf("Greetings from thread #%d! \n",threadNumber);
		usleep(500*1000); /* 500 ms sleep */
	}		
}

int main()
{
	pthread_t thread_1;
	pthread_t thread_2;
	
	pthread_create(&thread_1,NULL,threadFunction,0x01);
	pthread_create(&thread_2,NULL,threadFunction,0x02);
	
	pthread_tryjoin_np(thread_1,0x01);
	pthread_tryjoin_np(thread_2,0x02);

	while(1)
	{
		printf("Greetings from main loop!\n");
		usleep(1000*1000); /* 1000 ms sleep */
	}

}

/******************************************************************************
 * Async input / output from a simple C application
 *
 * Compile command: gcc level1.c -o main -lpthread
 *
 * ihsan Kehribar - 2013
 *****************************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

volatile char buff[128];	
volatile int newMessageFlag;

void *threadFunction(void *arg)
{	
	int i;
	char temp;

	while(1)
	{
		i = 0;
		temp = 0;
		while((temp!='\n')&(i<(sizeof(buff)-1)))
		{
			temp = getchar();
			buff[i] = temp;
			i++;
		}
		newMessageFlag = 1;
	}		
}

int main()
{
	int count = 0;
	pthread_t thread_1;
	pthread_create(&thread_1,NULL,threadFunction,NULL);	

	newMessageFlag = 0;

	while(1)
	{
		if(newMessageFlag)
		{
			newMessageFlag = 0;
			printf("Read: %s", buff);
		}
		printf("Main: %d\n",count++);
		usleep(1000*1000); /* 1 sec sleep */
	}

}


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

void *threadFunction(void *arg)
{	
	int i;
	char temp;
	char buff[128];	

	while(1)
	{
		i = 0;
		temp = 0;
		while(temp!='\n')
		{
			temp = getchar();
			buff[i] = temp;
			i++;
		}
		printf("Read: %s", buff);
	}		
}

int main()
{
	int count = 0;
	pthread_t thread_1;
	pthread_create(&thread_1,NULL,threadFunction,NULL);	

	while(1)
	{
		printf("Main: %d\n",count++);
		usleep(1000*1000); /* 1 sec sleep */
	}

}


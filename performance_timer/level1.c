/******************************************************************************
 * Basiic performance timer example
 *
 * Compile command: gcc level1.c -o main -lrt
 *
 * ihsan Kehribar - 2013
 *****************************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

int main()
{
	struct timespec start;
	struct timespec end;

	clock_gettime(CLOCK_REALTIME,&start);
	clock_gettime(CLOCK_REALTIME,&end);

	long int diff;
	diff = end.tv_nsec - start.tv_nsec;
	printf("Base offset: %d ns\n",diff);
	
	clock_gettime(CLOCK_REALTIME,&start);
	usleep(5000);
	clock_gettime(CLOCK_REALTIME,&end);

	diff = end.tv_nsec - start.tv_nsec;
	printf("usleep(5000) => %d ns\n",diff);
	
	return 0;
}

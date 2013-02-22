/******************************************************************************
 * FIR filter implementation in C
 *
 *	compile command: gcc level1.c -o main
 *
 * ihsan kehribar - 2013
 *****************************************************************************/
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

/* Filter related definitions */
#define N 100 
float xcirc[N]; 
float B_fir[N];
uint16_t newest = 0;

void initFir()
{
	uint16_t q;
	
	/* Initialize all the samples with zero */
	for(q=0;q<N;q++)
	{
		xcirc[q] = 0.0f;
		B_fir[q] = 0.01f; /* Meaningless coefficients ... */
	}
}

float updateFir(float newSample)
{
	uint16_t k;
	float temp1;
	float temp2;
	int32_t x_index;
	float mul_result;
	float accumulator = 0.0f;
	
	/* Circularly increment newest index */
	++newest;

	/* Fold if neccessary ... */
	if(newest == N)
	{
		newest = 0;
	}

	/* Put new sample in delay line */
	xcirc[newest] = newSample;

	/* Do convolution sum */
	x_index = newest;
	for(k = 0; k < N; k++)
	{
		/* Do the math ... */
		temp1 = B_fir[k];
		temp2 = xcirc[x_index];
		mul_result = temp1 * temp2;
		accumulator += mul_result;	
	
		/* Circularly decrement x_index */
		--x_index;

		/* Fold if neccessary */
		if(x_index == -1)
		{
			x_index = N-1;
		}
	}

	/* Return the filter result ... */
	return accumulator;
}	

int main()
{
	float output;
	float input = 0.0f;

	initFir();
	printf("Hi there!\n");

	while(1)
	{
		input += 1.0f;
		output = updateFir(input);
		printf("#i: %f - #o: %f\n",input,output);
	}
}


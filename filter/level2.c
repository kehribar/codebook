/******************************************************************************
 * FIR filter implementation in C
 *
 * compile command: gcc level2.c -o main
 *
 * ihsan kehribar - 2013
 *****************************************************************************/
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

/* Filter related definitions */
#define N 10 
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
		B_fir[q] = 0.1f; /* Meaningless coefficients ... */
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
	int r;
	FILE *raw;
	FILE *filtered;
	float temp_input;
	float temp_output;
	int lineNumber = 0;
	char inputBuffer[512];

	initFir();
	printf("Hi there!\n");

	raw = fopen("raw.txt","r");
	if(raw == NULL)
	{
		printf("Couldn't open the input file!\n");
		return -1;
	}

	filtered = fopen("output.txt","w");
	if(filtered == NULL)
	{
		printf("Couldn't create the output file!\n");
		return -1;
	}

	while(1)
	{
		/* Read new line ... */
		if(fgets(inputBuffer,sizeof(inputBuffer),raw) != NULL)
		{
			sscanf(inputBuffer,"%f\n",&temp_input);
			temp_output = updateFir(temp_input);
			fprintf(filtered,"%f\n",temp_output);
			printf("#l:%d #i:%f #o:%f\n",lineNumber++,temp_input,temp_output);
		}
		else
		{
			break;
		}
	}

	return 0;
}


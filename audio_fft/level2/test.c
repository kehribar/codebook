/******************************************************************************
 * Real time frequency analysis ...
 * Work in progress.
 *
 * ihsan kehribar - 2013
 *****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "portaudio.h"
#include <stdlib.h>
#include <stdio.h>
#include <fftw3.h>
#include <math.h>
#define PI 3.14159265358979
#define N 8192
#define REAL 0
#define IMAG 1
#define F_SAMP 44100
#define F_TEST 256
#define SAMPLE_RATE  F_SAMP
#define FRAMES_PER_BUFFER (512)
#define NUM_CHANNELS    (1)
#define NUM_SECONDS     (15)

int main(void)
{
    PaStreamParameters inputParameters;
    PaStream *stream = NULL;
    PaError err;
    float *sampleBlock;
    int i;
    int numBytes;    

    fftw_complex *in, *out, *prein; /* pointers for FFT input, output */
    fftw_plan my_plan; /* store the type of FFT we want to perform */
    in = (fftw_complex*) fftw_malloc(sizeof(fftw_complex)*N); /* memory allocation */
    prein = (fftw_complex*) fftw_malloc(sizeof(fftw_complex)*N); /* memory allocation */
    out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex)*N); /* pointers in and out are of type fftw_complex*/
    my_plan = fftw_plan_dft_1d(N, in, out, FFTW_FORWARD, FFTW_ESTIMATE); /* create the FFT plan */

    double power;
    double amplitude;
    double frequency;
    double freq_step = (double)F_SAMP / (double)N;
    
    printf("patest_read_write_wire.c\n"); fflush(stdout);

    numBytes = FRAMES_PER_BUFFER * sizeof(paFloat32) ;
    sampleBlock = (float *) malloc( numBytes );
    if( sampleBlock == NULL )
    {
        printf("Could not allocate record array.\n");
        exit(1);
    }

    err = Pa_Initialize();
    if( err != paNoError ) goto error;

    inputParameters.device = Pa_GetDefaultInputDevice();
    inputParameters.channelCount = 1;
    inputParameters.sampleFormat = paFloat32;
    inputParameters.suggestedLatency = Pa_GetDeviceInfo( inputParameters.device )->defaultHighInputLatency ;
    inputParameters.hostApiSpecificStreamInfo = NULL;

    FILE* record;
    record = fopen("record.txt","w");
    err = Pa_OpenStream(
              &stream,
              &inputParameters,
              NULL,
              SAMPLE_RATE,
              FRAMES_PER_BUFFER,
              paClipOff, 
              NULL, /* no callback, use blocking API */
              NULL /* no callback, so no callback userData */
    );
    if( err != paNoError ) goto error;

    err = Pa_StartStream( stream );
    if( err != paNoError ) goto error;
    
    for(i=0;i<N;i++)
    {
      prein[i][REAL] = 0;
      prein[i][IMAG] = 0;
    }

    printf("Wire on. Will run %d seconds.\n", NUM_SECONDS); fflush(stdout);
    for( i=0; i<(NUM_SECONDS*SAMPLE_RATE)/FRAMES_PER_BUFFER; ++i )
    {
       err = Pa_ReadStream( stream, sampleBlock, FRAMES_PER_BUFFER );
       if( err ) goto xrun;

      int i = 0;
      for(i=0;i<(N-FRAMES_PER_BUFFER);i++)
      {
        prein[i+FRAMES_PER_BUFFER][REAL] = prein[i][REAL];
        prein[i+FRAMES_PER_BUFFER][IMAG] = 0;
      }

      for(i=0;i<FRAMES_PER_BUFFER;i++)
      {
        prein[i][REAL] = sampleBlock[i];
        prein[i][IMAG] = 0;
      }

      for(i=0;i<N;i++)
      {
        in[i][REAL] = prein[i][REAL] * 1.0;
        in[i][IMAG] = 0;
      }

      fftw_execute(my_plan);

      printf("\033[2J\033[1;1H"); /* clear screen, go to top left */
      fflush(stdout);

      for(i=0;i<((N/100)+1);i++)
      {
        power = (out[i][0]*out[i][0]) + (out[i][1]*out[i][1]); 
        amplitude = sqrt(power);
        amplitude = amplitude/(N/2);
        frequency = (float)i*freq_step;        
        printf("\t%5.3f  %1.6f\t",frequency,amplitude);
        if((i%4)==0)
          printf("\n");
      }

    }
    
    err = Pa_StopStream( stream );
    if( err != paNoError ) goto error;

    free( sampleBlock );
    Pa_Terminate();
    
    return 0;

xrun:
    if( stream ) {
       Pa_AbortStream( stream );
       Pa_CloseStream( stream );
    }
    free( sampleBlock );
    Pa_Terminate();
    if( err & paInputOverflow )
       fprintf( stderr, "Input Overflow.\n" );
    if( err & paOutputUnderflow )
       fprintf( stderr, "Output Underflow.\n" );
    return -2;

error:
    if( stream ) {
       Pa_AbortStream( stream );
       Pa_CloseStream( stream );
    }
    free( sampleBlock );
    Pa_Terminate();
    fprintf( stderr, "An error occured while using the portaudio stream\n" );
    fprintf( stderr, "Error number: %d\n", err );
    fprintf( stderr, "Error message: %s\n", Pa_GetErrorText( err ) );
    return -1;
}
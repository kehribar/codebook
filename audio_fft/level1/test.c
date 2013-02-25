/******************************************************************************
 * Simple FFTW3 example. Fills the syntetic input buffer with pure sinusodial
 * wave samples and takes the FFT. 
 *
 * ihsan Kehribar - 2013
 *****************************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <fftw3.h>
#include <math.h>

#define PI 3.14159265358979
#define N 128
#define REAL 0
#define IMAG 1
#define F_SAMP 1024
#define F_TEST 256

int main(void)
{
  fftw_complex *in, *out; /* pointers for FFT input, output */
  fftw_plan my_plan; /* store the type of FFT we want to perform */
  in = (fftw_complex*) fftw_malloc(sizeof(fftw_complex)*N); /* memory allocation */
  out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex)*N); /* pointers in and out are of type fftw_complex*/
  my_plan = fftw_plan_dft_1d(N, in, out, FFTW_FORWARD, FFTW_ESTIMATE); /* create the FFT plan */

  /* fill the input buffer with syntetic waveform samples */
  int i = 0;
  for(i=0;i<N;i++)
  {
    in[i][REAL] = 1.1 * sin(2*PI*F_TEST*i/F_SAMP);
    in[i][IMAG] = 0;
  }

  fftw_execute(my_plan); /* performs the FFT stored in my_plan */

  /* create the frequency - amplitude representation from the FFT output */
  double power;
  double amplitude;
  double frequency;
  double freq_step = (double)F_SAMP / (double)N;
  for(i=0;i<((N/2)+1);i++)
  {
    power = (out[i][0]*out[i][0]) + (out[i][1]*out[i][1]); 
    amplitude = sqrt(power);
    amplitude = amplitude/(N/2);
    frequency = (float)i*freq_step;
    printf("%.3f  %f\n",frequency,amplitude);   
  }

  /* clear the memory */
  fftw_destroy_plan(my_plan);
  fftw_free(in); 
  fftw_free(out);

  return 0;
}

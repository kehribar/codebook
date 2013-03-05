/**************************************************************************
* Real time FFT + time domain graph with openGL
*
* ihsan Kehribar - March 2013
**************************************************************************/
#include <stdlib.h>
#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "portaudio.h"
#include "fdacoefs.h" // Coefficients for 44.1kHz sample rate, ~2kHz cutoff low pass
#include "fftw3.h"

#define N 2048
#define REAL 0
#define IMAG 1
#define F_SAMP 44100
#define F_TEST 256
#define SAMPLE_RATE  F_SAMP
#define FRAMES_PER_BUFFER 512
#define FIRSIZE 101 
#define FILTER_ENABLE 1
#define SUBSAMPLE 8
#define SAMPLECOUNT (FRAMES_PER_BUFFER / SUBSAMPLE)
#define PI 3.14159265358979

/* default window size */
const int HEIGHT = 256;
const int WIDTH = 800;

int errCount = 0;
float xcirc[FIRSIZE]; 
uint16_t newest = 0;
float sampleBlock_sub[SAMPLECOUNT];
int i,q,t;
double power;
double amplitude;
double freq_step = (double)F_SAMP / (double)N / SUBSAMPLE;
PaStreamParameters inputParameters;
PaStream *stream = NULL;
PaError err;
float *sampleBlock;
int numBytes;    
fftw_complex *in, *out, *prein; /* pointers for FFT input, output */
fftw_plan my_plan; /* store the type of FFT we want to perform */
float hammingWindow[N];
float powArray[N];
char printBuffer[128];

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
    if(newest == FIRSIZE)
    {
        newest = 0;
    }

    /* Put new sample in delay line */
    xcirc[newest] = newSample;

    /* Do convolution sum */
    x_index = newest;
    for(k = 0; k < FIRSIZE; k++)
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
            x_index = FIRSIZE-1;
        }
    }

    /* Return the filter result ... */
    return accumulator;
}   

void createHamming(float* array,int size)
{
	int i = 0;
	for(i=0;i<size;i++)
	{
		array[i] = 0.54 - (0.46 * cos((2*PI*i)/(size-1)));
	}
}

static void idle_function(void)
{
	err = Pa_ReadStream( stream, sampleBlock, FRAMES_PER_BUFFER );
	
	if(err != paNoError)
		printf("Error count: %d\tProblem: %s\n",++errCount,Pa_GetErrorText(err));

   #if FILTER_ENABLE
	   /* apply the low pass filter */
	   for(i=0;i<FRAMES_PER_BUFFER;i++)
	   {
	     sampleBlock[i]= updateFir(sampleBlock[i]);
	   }
   #endif

   /* subsampling ... */
   for(i=0;i<SAMPLECOUNT;i++)
   {
     sampleBlock_sub[i] = sampleBlock[SUBSAMPLE*i];
   }

   /* shift the input buffer */
   for(i=0;i<(N-SAMPLECOUNT);i++)
   {
     prein[i][REAL] = prein[i+SAMPLECOUNT][REAL];
     prein[i][IMAG] = 0;
   }

   /* add the new samples */
   for(i=N-SAMPLECOUNT;i<N;i++)
   {
     prein[i][REAL] = sampleBlock_sub[i-N+SAMPLECOUNT];
     prein[i][IMAG] = 0;
     //fprintf(record,"%f\n",sampleBlock_sub[i-N+SAMPLECOUNT]);
   }

   /* apply the window function to the samples */
   for(i=0;i<N;i++)
   {
     in[i][REAL] = prein[i][REAL] * hammingWindow[i];
     in[i][IMAG] = 0;
   }

   /* run the fft */
   fftw_execute(my_plan);

   for(i=0;i<((N/2)+1);i++)
   {
     power = (out[i][0]*out[i][0]) + (out[i][1]*out[i][1]); 
     amplitude = sqrt(power);
     amplitude = amplitude/(N/2);
     amplitude *= 1000; // arbitrary gain
     powArray[i]=amplitude;   
   }
      
	glutPostRedisplay();
}

// Here is the function 
void glutPrint(float x, float y, char* text, float r, float g, float b, float a)
{ 
    if(!text || !strlen(text)) return; 
    bool blending = false; 
    if(glIsEnabled(GL_BLEND)) blending = true; 
    glEnable(GL_BLEND); 
    glColor4f(r,g,b,a); 
    glRasterPos2f(x,y); 
    while (*text) { 
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, *text); 
        text++; 
    } 
    if(!blending) glDisable(GL_BLEND); 
}  

static void display_function(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);       
	glLoadIdentity();       

	sprintf(printBuffer,"Window witdh: %.2f Sec",(float)N/((float)F_SAMP/(float)SUBSAMPLE));
	glutPrint(WIDTH/128, (7*HEIGHT/8)-15, printBuffer, 0.85f, 0.85f, 0.85f, 0.0f);
	sprintf(printBuffer,"Freq step: %.2f Hz",freq_step);
	glutPrint(WIDTH/128, (7*HEIGHT/8), printBuffer, 0.85f, 0.85f, 0.85f, 0.0f);
	sprintf(printBuffer,"F_sampling: %.2f Hz",(float)F_SAMP/SUBSAMPLE);
	glutPrint(WIDTH/128, (7*HEIGHT/8)+15, printBuffer, 0.85f, 0.85f, 0.85f, 0.0f);
	
	
	int x_step = 1;
	int step = (N/2) / WIDTH;
	int gain = 10;

	glBegin(GL_LINES);     

		for(i=0;i<WIDTH;i+=x_step)
		{
			glColor3f ( 1.0, 0.0, 0.0);       
			glVertex2f( i, gain * powArray[i*step]);         
			glVertex2f( i+x_step, gain * powArray[(i+x_step)*step]);       
		
			glColor3f ( 0.0, 0.0, 0.0);       
			glVertex2f( i, (HEIGHT/2) + (100 * gain * prein[i*step/2][REAL]));         
			glVertex2f( i+x_step, (HEIGHT/2) + (100 * gain * prein[(i+x_step)*step/2][REAL]));  
		}

	glEnd();

	//glFlush();
	glutSwapBuffers();
}

void myinit(void)
{
	glClearColor(0.7, 0.7, 0.7, 0.0); // gray background
	glMatrixMode(GL_PROJECTION);      
	glLoadIdentity();                 
	gluOrtho2D( 0, WIDTH, 0, HEIGHT); // defining the corner points of the window
	glMatrixMode(GL_MODELVIEW);       
}

int main(int argc, char** argv)
{	
	in = (fftw_complex*) fftw_malloc(sizeof(fftw_complex)*N); /* memory allocation */
	prein = (fftw_complex*) fftw_malloc(sizeof(fftw_complex)*N); /* memory allocation */
	out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex)*N); /* pointers in and out are of type fftw_complex*/
	my_plan = fftw_plan_dft_1d(N, in, out, FFTW_FORWARD, FFTW_ESTIMATE); /* create the FFT plan */

	numBytes = FRAMES_PER_BUFFER * sizeof(paFloat32) ;
	sampleBlock = (float *) malloc( numBytes );
	
	err = Pa_Initialize();

	inputParameters.device = Pa_GetDefaultInputDevice();
	inputParameters.channelCount = 1;
	inputParameters.sampleFormat = paFloat32;
	inputParameters.suggestedLatency = Pa_GetDeviceInfo( inputParameters.device )->defaultLowInputLatency ;
	inputParameters.hostApiSpecificStreamInfo = NULL;

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

	err = Pa_StartStream( stream );

	createHamming(hammingWindow,N);

	for(i=0;i<N;i++)
	{
		prein[i][REAL] = 0;
		prein[i][IMAG] = 0;
	}

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutCreateWindow("Real time FFT graph");
	glutIdleFunc(&idle_function);
	glutDisplayFunc(&display_function);

	myinit();
	
	glutMainLoop();
	
	return 0;
}


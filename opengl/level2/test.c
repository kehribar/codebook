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
#include "fftw3.h"

#define N 16384
#define REAL 0
#define IMAG 1
#define F_SAMP 1000
#define SAMPLECOUNT 100
#define PI 3.14159265358979

/* default window size */
const int HEIGHT = 450;
const int WIDTH = 1000;

int i,q,t;
int win_max;
int win_min;
int is_paused;
int high_pass;
int win_ripple;
double win_rms;
double win_mean;
float powArray[N];
float *sampleBlock;
char printBuffer[128];
float hammingWindow[N];
float freq_gain = 0.005f;
float time_gain = 0.001f;
int xCursor1, xCursor2, yCursor1, yCursor2;
double freq_step = (double)F_SAMP / (double)N ;

fftw_complex *in, *out, *prein; /* pointers for FFT input, output */
fftw_plan my_plan; /* store the type of FFT we want to perform */

void createHamming(float* array,int size)
{
	int i = 0;
	for(i=0;i<size;i++)
	{
		array[i] = 0.54 - (0.46 * cos((2*PI*i)/(size-1)));
	}
}

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

float max = 65335;
float val = 0.0;
int direction = 1;

static void idle_function(void)
{
	double power;
	double amplitude;
	
	/* add the new inputs ... */
	for(i=0;i<SAMPLECOUNT;i++)
	{
		sampleBlock[i] = val;
		
		if(val>max)
			direction = 0;
		
		if(val<0)
			direction = 1;
			
		if(direction)
			val += 100;
		else
			val -= 100;
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
		prein[i][REAL] = sampleBlock[i-N+SAMPLECOUNT];
		prein[i][IMAG] = 0;
		#if 0
			fprintf(record,"%f\n",sampleBlock_sub[i-N+SAMPLECOUNT]);
		#endif
	}

	/* find the max,min values of the buffer ...*/
	win_rms = 0;
	win_max = 0;
	win_mean = 0;
	win_min = 65535;
	for(i=0;i<N;i++)
	{
		if(prein[i][REAL]>win_max)
			win_max = prein[i][REAL];
		if(win_min>prein[i][REAL])
			win_min = prein[i][REAL];
		win_rms += (prein[i][REAL]*prein[i][REAL]);
		win_mean += prein[i][REAL];
	}	
	win_rms /= (float)N;	
	win_mean /= (float)N;
	win_rms = sqrt(win_rms);		
	win_ripple = win_max - win_min;

	/* apply the window function to the samples */
	for(i=0;i<N;i++)
	{
		in[i][REAL] = prein[i][REAL] * hammingWindow[i];
		in[i][IMAG] = 0;
	}

	/* run the fft */
	fftw_execute(my_plan);

	/* calculate the power - amplitude array */
	for(i=0;i<((N/2)+1);i++)
	{
		power = (out[i][0]*out[i][0]) + (out[i][1]*out[i][1]); 
		amplitude = sqrt(power);
		amplitude = amplitude/(N/2);
		powArray[i] = amplitude;   
	}
	
	if(high_pass)
	{
		for(i=0;i<30;i++)
		{
			powArray[i] = 0x00;
		}
	}
	
	/* go to draw function */
	glutPostRedisplay();
}

void myMouseFunc( int button, int state, int x, int y ) 
{
	printf("button: %d state: %d x_pos: %d y_pos: %d\n",button,state,x,y);
	if(button == 0)
	{
		xCursor1 = x;
		yCursor1 = y;
	}
	else if(button==2)
	{
		xCursor2 = x;
		yCursor2 = y;
	}
}

#define FREQ_GAIN_FINE 0.001
#define FREQ_GAIN_COARSE 0.01
#define TIME_GAIN_FINE 0.0001
#define TIME_GAIN_COARSE 0.001

void keyb(unsigned char key, int x, int y)
{
	/* space bar to reset cursors */
	if(key==32)
	{
		xCursor1 = 0;
		xCursor2 = 0;
		yCursor1 = 0;
		yCursor2 = 0;
	}
	
	if(key=='p')
	{
		is_paused = !is_paused;
	}
	
	if(key=='q')
	{
		high_pass = !high_pass;
	}
	
	/* frequency plot gain adjustment ... */
	if(key=='z')
	{
		freq_gain += FREQ_GAIN_FINE;
	}
	if(key=='x')
	{
		freq_gain -= FREQ_GAIN_FINE;
		if(freq_gain<=0)
			freq_gain = FREQ_GAIN_FINE;
	}
	if(key=='c')
	{
		freq_gain += FREQ_GAIN_COARSE;
	}
	if(key=='v')
	{
		freq_gain -= FREQ_GAIN_COARSE;
		if(freq_gain<=0)
			freq_gain = FREQ_GAIN_COARSE;
	}

	/* time plot gain adjustment ... */
	if(key=='a')
	{
		time_gain += TIME_GAIN_FINE;
	}
	if(key=='s')
	{
		time_gain -= TIME_GAIN_FINE;
		if(time_gain<=0)
			time_gain = TIME_GAIN_FINE;
	}
	if(key=='d')
	{
		time_gain += TIME_GAIN_COARSE;
	}
	if(key=='f')
	{
		time_gain -= TIME_GAIN_COARSE;
		if(time_gain<=0)
			time_gain = TIME_GAIN_FINE;
	}
	
}

static void display_function(void)
{
	int x_step = 1;
	int step = (N/2) / WIDTH;
	int gain = 5;
	int maximum = 0;
	int max_index;
	int BASE = 2;
	
	if(!is_paused)
	{
		glClear(GL_COLOR_BUFFER_BIT);
		glMatrixMode(GL_MODELVIEW);       
		glLoadIdentity();       

		sprintf(printBuffer,"Freq step: %.2f Hz",freq_step);
		glutPrint(WIDTH/128, HEIGHT-15, printBuffer, 0.89f, 0.89f, 0.89f, 0.0f);
		sprintf(printBuffer,"F_sampling: %.2f Hz",(float)F_SAMP);
		glutPrint(WIDTH/128, HEIGHT-30, printBuffer, 0.89f, 0.89f, 0.89f, 0.0f);
		sprintf(printBuffer,"Window witdh: %.2f Sec",(float)N/((float)F_SAMP));
		glutPrint(WIDTH/128, HEIGHT-45, printBuffer, 0.89f, 0.89f, 0.89f, 0.0f);

		glBegin(GL_LINES);    
		
			/* Start drawing cursors ... */
			glColor3f ( 0.95, 0.85, 0.85);       
			glVertex2f( xCursor1, 0);         
			glVertex2f( xCursor1, HEIGHT/4); 

			glColor3f ( 0.75, 0.85, 0.85);       
			glVertex2f( xCursor2, 0);         
			glVertex2f( xCursor2, HEIGHT/4);  

			/* draw the zero line for frequency domain ... */
			glColor3f ( 0.75, 0.75, 0.75);       
			glVertex2f( 0, BASE);         
			glVertex2f( WIDTH, BASE); 
			
			/* draw the zero line for time domain ... */
			glColor3f ( 0.75, 0.75, 0.75);         
			glVertex2f( 0, (HEIGHT/2));         
			glVertex2f( WIDTH, (HEIGHT/2));
			
			/* draw the max line for time domain ... */
			glColor3f ( 0.75, 0.75, 0.75);       
			glVertex2f( 0, (65536*time_gain)+(HEIGHT/2));         
			glVertex2f( WIDTH, (65536*time_gain)+(HEIGHT/2));

			for(i=0;i<WIDTH;i+=x_step)
			{
				/* frequency domain line ... */
				glColor3f ( 1.0, 0.0, 0.0);       
				glVertex2f( i, BASE + (freq_gain * powArray[i*step]));         
				glVertex2f( i+x_step, BASE + (freq_gain * powArray[(i+x_step)*step])); 

				/* try to find the maximum */
				if(powArray[i*step]>maximum)
				{
					maximum = powArray[i*step];
					max_index = i*step;
				}      
				
				/* time domain line ... */
				glColor3f ( 0.5, 0.5, 0.5);       
				glVertex2f( i, (HEIGHT/2) + (time_gain * prein[i*step*2][REAL]));         
				glVertex2f( i+x_step, (HEIGHT/2) + (time_gain * prein[(i+x_step)*step*2][REAL]));  
			}		

			/* maximum value indicator in frequency domain ... */
			glColor3f ( 0.85, 0.85, 0.85);       
			glVertex2f( (max_index/step), (freq_gain * powArray[max_index]));         
			glVertex2f( (max_index/step), 20 + (freq_gain * powArray[max_index])); 

		glEnd();

		/* min,max,ripple information text ... */
		sprintf(printBuffer,"MAX: %d",win_max);
		glutPrint(WIDTH-100, HEIGHT - 20, printBuffer, 0.89f, 0.89f, 0.89f, 0.0f);
		sprintf(printBuffer,"MIN: %d",win_min);
		glutPrint(WIDTH-100, HEIGHT - 35, printBuffer, 0.89f, 0.89f, 0.89f, 0.0f);
		sprintf(printBuffer,"RIPPLE: %d",win_ripple);
		glutPrint(WIDTH-100, HEIGHT - 50, printBuffer, 0.89f, 0.89f, 0.89f, 0.0f);
		sprintf(printBuffer,"RMS: %d",(int)win_rms);
		glutPrint(WIDTH-100, HEIGHT - 65, printBuffer, 0.89f, 0.89f, 0.89f, 0.0f);
		sprintf(printBuffer,"MEAN: %d",(int)win_mean);
		glutPrint(WIDTH-100, HEIGHT - 80, printBuffer, 0.89f, 0.89f, 0.89f, 0.0f);
		
		/* maximum value information text */
		sprintf(printBuffer,"Amplitude: %f",powArray[max_index]);
		glutPrint(max_index/step, 30 + (freq_gain * powArray[max_index]), printBuffer, 0.05f, 0.05f, 0.05f, 0.0f);
		sprintf(printBuffer,"Peak freq: %.2f Hz",(float)max_index*freq_step);
		glutPrint((max_index/step), 50 + (freq_gain * powArray[max_index]), printBuffer, 0.05f, 0.05f, 0.05f, 0.0f);

		int currsor1_bottom;

		if(xCursor1 != 0)
		{
			sprintf(printBuffer,"Cursor Amplitude: %f",powArray[xCursor1*step]);
			glutPrint(WIDTH/128,HEIGHT-60, printBuffer, 0.95f, 0.85f, 0.85f, 0.0f);
			sprintf(printBuffer,"Cursor freq: %.2f Hz",(float)xCursor1*freq_step*step);
			glutPrint(WIDTH/128,HEIGHT-75, printBuffer, 0.95f, 0.85f, 0.85f, 0.0f);
			currsor1_bottom = 75;	
		}
		else
		{
			sprintf(printBuffer,"Cursor Off");
			glutPrint(WIDTH/128,HEIGHT-60, printBuffer, 0.95f, 0.85f, 0.85f, 0.0f);
			currsor1_bottom = 60;
		}

		if(xCursor2 != 0)
		{
			sprintf(printBuffer,"Cursor Amplitude: %f",powArray[xCursor2*step]);
			glutPrint(WIDTH/128,HEIGHT-(currsor1_bottom+15), printBuffer, 0.75f, 0.85f, 0.85f, 0.0f);
			sprintf(printBuffer,"Cursor freq: %.2f Hz",(float)xCursor2*freq_step*step);
			glutPrint(WIDTH/128,HEIGHT-(currsor1_bottom+30), printBuffer, 0.75f, 0.85f, 0.85f, 0.0f);
		}
		else
		{
			sprintf(printBuffer,"Cursor Off");
			glutPrint(WIDTH/128,HEIGHT-(currsor1_bottom+15), printBuffer, 0.75f, 0.85f, 0.85f, 0.0f);
		}

		glutSwapBuffers();
	}
}

void myinit(void)
{
	glClearColor(0.7, 0.7, 0.7, 0.0);
	glMatrixMode(GL_PROJECTION);      
	glLoadIdentity();                 
	gluOrtho2D( 0, WIDTH, 0, HEIGHT);
	glMatrixMode(GL_MODELVIEW);
	is_paused = 0;       
	high_pass = 0;
}

int main(int argc, char** argv)
{	
	in = (fftw_complex*) fftw_malloc(sizeof(fftw_complex)*N); /* memory allocation */
	prein = (fftw_complex*) fftw_malloc(sizeof(fftw_complex)*N); /* memory allocation */
	out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex)*N); /* pointers in and out are of type fftw_complex*/
	my_plan = fftw_plan_dft_1d(N, in, out, FFTW_FORWARD, FFTW_ESTIMATE); /* create the FFT plan */
	sampleBlock = (float *) malloc( N * sizeof(float) );
	
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
	glutMouseFunc(&myMouseFunc);
	glutKeyboardFunc(keyb);
	glEnable(GL_BLEND);

	myinit();
	glutMainLoop();
	
	return 0;
}


#include <processor_include.h>
#include <sysreg.h>
#include <signal.h>
#include <string.h>
#include <filter.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <filter.h>
#include <complex.h>

#include "framework.h"

//#define N 1024

float sum(float in[], int length);
//float linspace(float a, float b, int steps);

void main()
{
  int N = 1024;
  float x1[N], x2[N], x3[N], x4[N];
  float v = 300.0;            //speed of sound
  float d = 0.04;             //distance between microphones
  int numberOfBuffers = 10;    //
  int Fs = 16000;              //Sample frequency
  float pi = 3.1415;
  float w[N];
  int i;
  for(i = 0; i < N; i++) {w[i] = (i * 2 * pi)/N;} 
  int resolution = 1000;
  float angles[resolution];
  for(i = 0; i < resolution; i++){angles[i] = i * (2*pi)/resolution;}

  FILE *fp1;
  FILE *fp2;
  FILE *fp3;
  FILE *fp4;

  fp1 = fopen("mic1.txt","r");
  fp2 = fopen("mic2.txt","r");
  fp3 = fopen("mic3.txt","r");
  fp4 = fopen("mic4.txt","r");
  
  printf("Reading file.\n");
  
  for(i = 0; i< N; i++)
    {
      fscanf(fp1, "%f",&x1[i]);
      fscanf(fp2, "%f",&x2[i]);
      fscanf(fp3, "%f",&x3[i]);
      fscanf(fp4, "%f",&x4[i]);
      //printf("%10.10f\n", x2[i]);
    }
     
  
  complex_float X1[N/2], X2[N/2], X3[N/2], X4[N/2];
  
  printf("Doing FFT.\n");
  
  printf("X1\n");
  rfft1024(x1, X1);
  printf("X2\n");
  rfft1024(x2, X2);
  printf("X3\n");
  rfft1024(x3, X3);
  printf("X4\n");
  rfft1024(x4, X4);
  
  for(i = 0; i < N/2; i++)
  {
  	printf("Index %d: X1: %f X2: %f\n", i, X1[i].re, X2[i].re);
  }
  
  printf("FFT finnished.\n");
  
  complex_float GA[N/2], GB[N/2], GC[N/2], GD[N/2];
  
  complex_float X1conj[N/2], X2conj[N/2], X3conj[N/2], X4conj[N/2];
  
  
  for(i = 0; i < N/2; i++)
  {
  	printf("%d\n", i);
  	X1conj[i] = conj(X1[i]);
  	printf("X1conj = %f + %fi\n", X1conj[i].re, X1conj[i].im);
  	complex_float z = cmltf(X2[i], X1conj[i]);
  	printf("GA = %f + %fi\n", z.re, z.im);
    GA[i] = z;
  	//printf("GA = %f + %fi\n", GA[i].re, GA[i].im);
  	//printf("%f + %fi\n", X1[i].re, X1[i].im);
  }
  printf("%10.10f\n",sum(x1,1024)); 
  
  //printf("%f + %f i", GA[511].re, GA[511].im);

}

float sum(float in[], int length)
{
  float res = 0;
  int i;
  for( i= 0; i < length; i++)
    {
      res += in[i];
    }

  return res;
	
	//dsp_init();
	//dsp_start();
	/*spi_init_slave();
	spi_send();
	//spi_read();*/
	//test();
	//spi_read();
}

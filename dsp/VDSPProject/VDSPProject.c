#include <processor_include.h>
#include <sysreg.h>
#include <signal.h>
#include <string.h>
#include <filter.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <complex.h>

#include "framework.h"

//GLOBAL VARIABLES
	int N = DSP_BLOCK_SIZE;			//Number of samples in each buffer
	int lenTmpVec;					//Length of vectors used in SRP-algorithm
  	int halfN;						//Half the length of the buffers used
  	int Fs = DSP_SAMPLE_RATE;       //Sample frequency
	int resolution = 360;			//Resulution in SRP-algorithm. The higher value the higher resolution.
  	int nbrOfPackages = 20;			//Number of packages (buffers) to collect before the SRP algorithm starts.
  	int packageCounter = 1;			//How many packages has been collected so far, reseted when the SRP algorithm calculates an angle
   	float conversionConstant;		//Constant used in SRP-algorithm
  	float prevAngle =0 ;
   	float pi = 3.14159265;			
  	float v = 330.0;            	//Speed of sound
  	float d = 0.039;             	//distance between microphones
  	float * angularFreq;						//Pointer to the normaized angular freqency
	float * angles;					//Pointer to the angles to be tested in SRP algorithm.
	float * x1;
	float * x2;
	float * x3;
	float * x4;
	complex_float * GA;				
	complex_float * GB;
	complex_float * GC;
	complex_float * GD;
	complex_float * GAmean;			//Pointers to the arrays containing cross spectrum, mean value of cross spectrum and FFT of the samples signal.
	complex_float * GBmean;
	complex_float * GCmean;
	complex_float * GDmean;
	complex_float * X1;
	complex_float * X2;
	complex_float * X3;
	complex_float * X4;

	void algorithm_setup(void);
	void algorithm_close(void);
	void algorithm(void);
	void new_algorithm(void);

void algorithm_close()
{
	/*
		Dealocates all memory used by the pointers.
	*/

  free(GA);
  free(GB);  
  free(GC);
  free(GD);
  
  free(GAmean);
  free(GBmean);
  free(GCmean);
  free(GDmean);
  
  free(x1);
  free(x2);
  free(x3);
  free(x4); 
  	
  free(X1);
  free(X2);
  free(X3);
  free(X4);
  
  free(angularFreq);
  free(angles);
  
  printf("Memory deallocation complete \n");
}

void algorithm_setup()
{
  //Initialize global variables
  halfN = N/2;
  conversionConstant  = Fs * d / v;
  lenTmpVec = N/2 - 9;
	
  //Memory allocation
  
  GA = (complex_float *) malloc(sizeof(complex_float)*halfN);
  GB = (complex_float *) malloc(sizeof(complex_float)*halfN);
  GC = (complex_float *) malloc(sizeof(complex_float)*halfN);
  GD = (complex_float *) malloc(sizeof(complex_float)*halfN);
  
  GAmean = (complex_float *) malloc(sizeof(complex_float)*halfN);
  GBmean = (complex_float *) malloc(sizeof(complex_float)*halfN);
  GCmean = (complex_float *) malloc(sizeof(complex_float)*halfN);
  GDmean = (complex_float *) malloc(sizeof(complex_float)*halfN);
  
  int i;
  for(i = 0; i < halfN;i++)
  {
  	GAmean[i].re = 0;
  	GBmean[i].re = 0;
  	GCmean[i].re = 0;
  	GDmean[i].re = 0;
  	GAmean[i].im = 0;
  	GBmean[i].im = 0;
  	GCmean[i].im = 0;
  	GDmean[i].im = 0;
  }
  
  angularFreq =  (float *) malloc(sizeof(float)*N);
  x1 = (float *) malloc(sizeof(float)*N);
  x2 = (float *) malloc(sizeof(float)*N);
  x3 = (float *) malloc(sizeof(float)*N);
  x4 = (float *) malloc(sizeof(float)*N);

  X1 = (complex_float *) malloc(sizeof(complex_float)*halfN);
  X2 = (complex_float *) malloc(sizeof(complex_float)*halfN);
  X3 = (complex_float *) malloc(sizeof(complex_float)*halfN);
  X4 = (complex_float *) malloc(sizeof(complex_float)*halfN);
  angles = (float *) malloc(sizeof(float)*resolution);
  
  if(GA != NULL && GB != NULL && GC != NULL && GD != NULL && GAmean != NULL && GBmean != NULL && GCmean != NULL && GDmean != NULL && angularFreq != NULL && x1 != NULL && x2 != NULL && x3 != NULL && x4 != NULL && X1 != NULL && X2 != NULL && X3 != NULL && X4 != NULL && angles != NULL)
  {
  	printf("Memory allocation finnished \n");
  } else 
  {
  	printf("Memory allocation failed \n");
  	return;
  }
 
   for(i = 0; i < resolution; i++)
  {
  	angles[i] = i * (2*pi)/resolution;
  }
  for(i = 0; i < N; i++) 
  {
  	angularFreq[i] = (i * 2 * pi)/N;
  }   
}

void algorithm()
{
  	int i;
  	
  	/*
  	Disable interrupts, load data from codecs
  	*/
  	
	interrupt(SIG_SP1,SIG_IGN);
	sample_t *u32 = dsp_get_audio_01();
	sample_t *u33 = dsp_get_audio_23();

	for(i = 0; i < N; i++)
	{
		x1[i] = (float) u32[i].left;
		x2[i] = (float) u32[i].right;
		x3[i] = (float) u33[i].left;
		x4[i] = (float) u33[i].right;
	}
	
	switch(N)
	{
	 case 64:
	 	rfft64(x1, X1);
	  	rfft64(x2, X2);
	  	rfft64(x3, X3);
	  	rfft64(x4, X4);
	  	break;
  	
	 case 128:
	 	rfft128(x1, X1);
	 	rfft128(x2, X2);
	 	rfft128(x3, X3);
	 	rfft128(x4, X4);
	 	break;
  	
     case 256:
	 	rfft256(x1, X1);
	 	rfft256(x2, X2);
	 	rfft256(x3, X3);
	 	rfft256(x4, X4);
	 	break;
  	
	 case 512:
	 	rfft512(x1, X1);
	 	rfft512(x2, X2);
	 	rfft512(x3, X3);
	 	rfft512(x4, X4);
	 	break;
  		
	 case 1024:
	 	rfft1024(x1, X1);
	 	rfft1024(x2, X2);
	 	rfft1024(x3, X3);
	 	rfft1024(x4, X4);
	 	break;
  		
	 case 2048:
	 	rfft2048(x1, X1);
	 	rfft2048(x2, X2);
	 	rfft2048(x3, X3);
	 	rfft2048(x4, X4);	
	 	break;
  		
	 default:
	 	printf("Invalid length of Fast Fourier Transform. Should be 64, 128, 256, 512, 1024 or 2048. \n");
		return;
     }
  
  	int n;
	for(n = 0; n < halfN; n++)
	{
	  
	/*
	  GA[n] = cmltf(X2[n], conj(X1[n]));
	  GB[n] = cmltf(X2[n], conj(X4[n]));
	  GC[n] = cmltf(X4[n], conj(X3[n]));
	  GD[n] = cmltf(X1[n], conj(X3[n]));
	 
	  
	  */
	  GA[n] = cmltf(X1[n], conj(X2[n]));
	  GB[n] = cmltf(X2[n], conj(X4[n]));
	  GC[n] = cmltf(X4[n], conj(X3[n]));
	  GD[n] = cmltf(X3[n], conj(X1[n]));
	 
	 
	}

	for(i = 0; i < halfN; i++)
	{
	  GAmean[i] = caddf(GA[i], GAmean[i]);
	  GBmean[i] = caddf(GB[i], GBmean[i]);
	  GCmean[i] = caddf(GC[i], GCmean[i]);
	  GDmean[i] = caddf(GD[i], GDmean[i]);
	}
  
  /*
  Calculate steered response power
  */
  
  packageCounter++;
  if(packageCounter -1 == nbrOfPackages)
  {
  	int k;
  	for (k = 0; k< halfN; k++)
  	{
  	GAmean[k].re = GAmean[k].re/nbrOfPackages;
	GAmean[k].im = GAmean[k].im/nbrOfPackages;
  	GBmean[k].re = GBmean[k].re/nbrOfPackages;
	GBmean[k].im = GBmean[k].im/nbrOfPackages;
	GCmean[k].re = GCmean[k].re/nbrOfPackages;
	GCmean[k].im = GCmean[k].im/nbrOfPackages;
	GDmean[k].re = GDmean[k].re/nbrOfPackages;
	GDmean[k].im = GDmean[k].im/nbrOfPackages;
  	}
	
	
	complex_float exponentCos1;
	exponentCos1.re = 0;
	complex_float exponentSin1;
	complex_float exponentCos2;
	exponentCos2.re = 0;
	complex_float exponentSin2;
	exponentSin2.re = 0; ;
	exponentSin1.re = 0; 
	complex_float sumTot;

  	int j;
  	float maxAngle = 0;
  	float maxVal = 0;
  	float tmpAngle;
  	complex_float amlt;
  	complex_float bmlt;
  	complex_float cmlt;
  	complex_float dmlt;
  	complex_float inputSin1;
  	complex_float inputCos1;
  	complex_float inputSin2;
  	complex_float inputCos2;
  	
  	for(j = 0; j<resolution; j++)
  	{
  		tmpAngle = angles[j];
  		sumTot.re = 0;
  		sumTot.im = 0;
	  	for(i = 9; i <lenTmpVec; i++)
	  	{
	  		
	  		exponentCos1.im = -1 * cosf(tmpAngle) * angularFreq[i] * conversionConstant;
	  		exponentSin1.im = -1 * sinf(tmpAngle) * angularFreq[i] * conversionConstant;
	  		exponentCos2.im = 1 * cosf(tmpAngle) * angularFreq[i] * conversionConstant;
	  		exponentSin2.im = 1 * sinf(tmpAngle) * angularFreq[i] * conversionConstant;
	  		
	  		
	  		inputSin1 = cexpf(exponentSin1);
	  		inputCos1 = cexpf(exponentCos1); 
	  		inputSin2 = cexpf(exponentSin2);
	  		inputCos2 = cexpf(exponentCos2); 
	  		
	  		amlt = cmltf(GAmean[i], inputCos1);
	  		bmlt = cmltf(GBmean[i], inputSin1);
	  		cmlt = cmltf(GCmean[i], inputCos2);
	  		dmlt = cmltf(GDmean[i], inputSin2);
	  		
	  		sumTot = caddf(sumTot, amlt);
	  		sumTot = caddf(sumTot, bmlt);
	  		sumTot = caddf(sumTot, cmlt);
	  		sumTot = caddf(sumTot, dmlt);
	  
	  	
	  	if(sumTot.re >= maxVal)
	  	{
	  		maxVal = sumTot.re;
	  		maxAngle = tmpAngle;
	  	}
	  	}
  	}
  	
  	/*
  	Send calculated angle with SPI
  	*/
  	

  	if(maxVal > 100) 
  	{	
	float trans  = 255/360.0*180/pi*maxAngle;
	spi_send((int) trans);
  	//printf("MaxVAL = %f\n", maxVal);
  	prevAngle = maxAngle;
  	}
  	
  	packageCounter = 1;
  	
  	/*
  	Reset mean values after SRP algorithm 
  	*/
  	
  	for (k = 0; k < halfN; k++)
  	{
  		GAmean[k].re = 0;
	  	GBmean[k].re = 0;
	  	GCmean[k].re = 0;
	  	GDmean[k].re = 0;
	  	GAmean[k].im = 0;
	  	GBmean[k].im = 0;
	  	GCmean[k].im = 0;
	  	GDmean[k].im = 0;
  	}
  }
}

void new_algorithm()
{
	interrupt(SIG_SP1,SIG_IGN);
	sample_t *u32 = dsp_get_audio_01();
	sample_t *u33 = dsp_get_audio_23();
	int i;
	for (i = 0; i < N; i++)
	{
		x1[i] = u32[i].left;
		x2[i] = u32[i].right;
	}	
	
	rfft256(x1,X1);
	rfft256(x2,X2);
	
	complex_float val = cmltf(X1[16], conjf(X2[16]));
	float angle = atan2f(val.im, val.re);
	float trans  = 255/360.0*180/pi*angle;
	spi_send((int) 170*255/360);
}


void main()
{
 	dsp_init();
	dsp_start();
	test();
	algorithm_setup();
	interrupt(SIG_SP1,algorithm);
	while(1)
	{
		interrupt(SIG_SP1,algorithm);
		idle();
	}
	algorithm_close();
}

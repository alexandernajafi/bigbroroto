#include <stdio.h>
#include <stdlib.h>
#include <fftw3.h>
#include <math.h>

float sum(float in[], int length);
float linspace(float a, float b, int steps);

int main(int argc, char *argv[])
{
  int bufferSize = 1024;
  float x1[bufferSize], x2[bufferSize], x3[bufferSize], x4[bufferSize];
  float v = 300.0;            //speed of sound
  float d = 0.04;             //distance between microphones
  int numberOfBuffers = 10;    //
  int Fs = 16000;              //Sample frequency
  float pi = 3.1415;
  float w[bufferSize];
  int i;
  for(i = 0; i < bufferSize; i++) {w[i] = (i * 2 * pi)/bufferSize;} 

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

  for(i = 0; i< bufferSize; i++)
    {
      fscanf(fp1, "%f",&x1[i]);
      fscanf(fp2, "%f",&x2[i]);
      fscanf(fp3, "%f",&x3[i]);
      fscanf(fp4, "%f",&x4[i]);
    }
    
  float GAmean, GBmean, GCmean, GDmean = 0;
  float X1[bufferSize][2], X2[bufferSize][2], X3[bufferSize][2], X4[bufferSize][2];

  printf("%f\n",sum(x1,1024)); 

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
}


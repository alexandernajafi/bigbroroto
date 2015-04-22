#include <stdio.h>
#include <stdlib.h>
#include <fftw3.h>
int main(int argc, char *argv[])
{
  double v = 300.0;            //speed of sound
  double d = 0.04;             //distance between microphones
  int numberOfBuffers = 10;    //
  int Fs = 16000;              //Sample frequency
  int bufferSize = 1024;

  FILE *fp1;
  FILE *fp2;
  FILE *fp3;
  FILE *fp4;

  fp1 = fopen("mic1.txt","r");
  fp2 = fopen("mic2.txt","r");
  fp3 = fopen("mic3.txt","r");
  fp4 = fopen("mic4.txt","r");

  double inputMic1[bufferSize];
  double inputMic2[bufferSize];
  double inputMic3[bufferSize];
  double inputMic4[bufferSize];

  int i;
  for(i = 0; i< bufferSize; i++)
    {
        fscanf(fp1, "%lf\n", &inputMic1[i]);
        fscanf(fp2, "%lf\n", &inputMic2[i]);
        fscanf(fp3, "%lf\n", &inputMic3[i]);
        fscanf(fp4, "%lf\n", &inputMic4[i]);
    }

}

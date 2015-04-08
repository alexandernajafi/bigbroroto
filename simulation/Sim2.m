clf
clear all
close all
clc

Fs = 8000;                          %Sample Frequency [Hz]
v = 330;                            %Speed of sound [m/s]
phi = 16*pi/10                      %XY-plane angle [rad]
theta = 0;                          %Z-axis angle [rad]
d = 0.04;                           %Distance between microphones [m]
maxFreq = 4000;                     %Highest allowed Freqency [Hz]

dtA = cos(phi)*d/v;               %Time difference between microphones in top horizontal pair
dtB = sin(phi)*d/v;               %Time difference between microphones in right vertical pair
dtC = cos(phi)*d/v;               %Time difference between microphones in bottom horizontal pair
dtD = sin(phi)*d/v;               %Time difference between microphones in left vertical pair

dphase1 = dtA*Fs;                   %Convert timedifference into fraction of sample frequency. Phase difference between microphone 2 and 1.
dphase2 = dtB*Fs;                   %Convert timedifference into fraction of sample frequency. Reference microphone.
dphase3 = dtC*Fs+dtB*Fs;            %Convert timedifference into fraction of sample frequency. Phase difference between microphone 2 and 3.
dphase4 = dtD*Fs;                   %Convert timedifference into fraction of sample frequency. Phase difference between microphone 2 and 4.

h1 = sinc((-10:10)-dphase1);        
h2 = sinc((-10:10)-0);             
h3 = sinc((-10:10)-dphase3);
h4 = sinc((-10:10)-dphase4);

x = randn(1024,1);                  %Random noise

x1 = filter(h1,1,x);
x2 = filter(h2,1,x);
x3 = filter(h3,1,x);
x4 = filter(h4,1,x);

X1 = fft(x1);
X2 = fft(x2);
X3 = fft(x3);
X4 = fft(x4);

GA = X2.*conj(X1);
GB = X2.*conj(X4);
GC = X4.*conj(X3);
GD = X1.*conj(X3);

w = (0:1023)*2*pi/1024;

plot(w,abs(GA))

maxvalue = 0;
maxtau = 0;
nbrofvalues = 1000;
alpha = linspace(0,2*pi,nbrofvalues);

for i = 1:nbrofvalues  
%totval = sum(GA(10:502).*exp(-1*j*w(10:502)'*d/v*cos(taus(i)))+GB(10:502).*exp(-1*j*w(10:502)'*d/v*sin(taus(i)))+GC(10:502).*exp(-1*j*w(10:502)'*d/v*cos(taus(i)))+GD(10:502).*exp(-1*j*w(10:502)'*d/v*sin(taus(i))));    
totval = sum(GA(10:502).*exp(-1*j*w(10:502)'*d/v*Fs*cos(alpha(i)))+GB(10:502).*exp(-1*j*w(10:502)'*d/v*Fs*sin(alpha(i)))+GC(10:502).*exp(-1*j*w(10:502)'*d/v*1*Fs*cos(alpha(i)))+GD(10:502).*exp(-1*j*w(10:502)'*d/v*Fs*sin(alpha(i))));    
if(totval > maxvalue)
        maxvalue = totval;
        maxtau = alpha(i);
end 
    
end

maxtau


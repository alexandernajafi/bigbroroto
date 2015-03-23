clf
clear all
close all
clc

Fs = 8000;                      %Sample Frequency [Hz]
v = 330;                        %Speed of sound [m/s]
azimutAngle = 0;             %Azimut angle [rad]
d = 0.04;                       %Distance between microphones [m]
maxFreq = 4000;                 %Highest allowed Freqency [Hz]

dtA = cos(azimutAngle)*d/v      %Time difference between microphones in top horizontal pair
dtB = sin(azimutAngle)*d/v;     %Time difference between microphones in right vertical pair
dtC = cos(azimutAngle)*d/v      %Time difference between microphones in bottom horizontal pair
dtD = sin(azimutAngle)*d/v;     %Time difference between microphones in left vertical pair

dphaseA = dtA/(1/Fs);           %Convert timedifference into fraction of sample frequency
dphaseB = dtB/(1/Fs);           %Convert timedifference into fraction of sample frequency
dphaseC = dtC/(1/Fs);           %Convert timedifference into fraction of sample frequency
dphaseD = dtD/(1/Fs);           %Convert timedifference into fraction of sample frequency

h1 = sinc((-10:10)-dphaseA);        
h2 = sinc((-10:10)+0);
h3 = sinc((-10:10)-dphaseC);
h4 = sinc((-10:10)-dphaseB);
h4b = sinc((-10:10)-0);

x = randn(1024,1);
x1 = filter(h1, 1, x);
x2 = filter(h2, 1, x);
x3 = filter(h3, 1, x);
x4 = filter(h4, 1, x);
x4b = filter(h4b, 1, x);

X1 = fft(x1);
X2 = fft(x2);
X3 = fft(x3);
X4 = fft(x4);
X4b = fft(x4b);

GA = X2 .* conj(X1);
GB = X2 .* conj(X4);
GC = X4b .* conj(X3);
w = (0:1023)*2*pi/1024;
 
maxvalueA = 0;
maxtauA = 0;
maxvalueB = 0;
maxtauB = 0;
maxvalueC = 0;
maxtauC = 0;

nbrofvalues = 1000;
taus = linspace(-1,1,nbrofvalues);

for i = 1:nbrofvalues  
    value1 = sum(GA(10:502).*exp(-1*j*w(10:502)'*taus(i)));
    value2 = sum(GB(10:502).*exp(-1*j*w(10:502)'*taus(i)));
    value3 = sum(GC(10:502).*exp(-1*j*w(10:502)'*taus(i)));

    if (value1 >= maxvalueA),
        maxvalueA = value1;
        maxtauA = taus(i); 
    end
    
    if (value2 >= maxvalueB),
        maxvalueB = value2;
        maxtauB = taus(i); 
    end
    
    if (value3 >= maxvalueC),
        maxvalueC = value3;
        maxtauC = taus(i); 
    end   
end

dtAM = maxtauA*(1/Fs)
dtBM = maxtauB*(1/Fs)
dtCM =maxtauC*(1/Fs)
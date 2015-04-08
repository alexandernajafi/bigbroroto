clf
clear all
close all
clc

sinc = @(x) sin(pi*x+1E-8)./(pi*x + 1E-8);

Fs = 8000;                      %Sample Frequency [Hz]
v = 330;                        %Speed of sound [m/s]
azimutAngle = 0.1;              %Azimut angle [rad]
d = 0.04;                       %Distance between microphones [m]
maxFreq = 4000;                 %Highest allowed Freqency [Hz]

%       h1              h2
%          Microphones
%       h3              h4



%Simulation of sound to microphone below

                                %Time difference between microphones
dtA = cos(azimutAngle)*d/v;     %Top horizontal pair
dtB = sin(azimutAngle)*d/v;     %Right vertical pair
dtC = cos(azimutAngle)*d/v;     %Bottom horizontal pair
dtD = sin(azimutAngle)*d/v;     %Left vertical pair

dphaseA = dtA/(1/Fs);           %Convert timedifference into fraction of sample frequency
dphaseB = dtB/(1/Fs);
dphaseC = dtC/(1/Fs);
dphaseD = dtD/(1/Fs);

h1 = sinc((-10:10)-dphaseA);    %Apply phasedifference to microphones
h2 = sinc((-10:10)-0);
h3 = sinc((-10:10)-dphaseB-dphaseC);
h4 = sinc((-10:10)-dphaseD);

x = randn(1024,1);              %Random noise

x1 = filter(h1,1,x);            %Apply random noise to microphones
x2 = filter(h2,1,x);
x3 = filter(h3,1,x);
x4 = filter(h4,1,x);

%Simulation Finnish

%Algorithm start

X1 = fft(x1);                   %Fouriertransform of microphones
X2 = fft(x2);
X3 = fft(x3);
X4 = fft(x4);

GA = X2.*conj(X1);
GB = X2.*conj(X4);
GC = X4.*conj(X3);
GD = X1.*conj(X3);

w = (0:1023)*2*pi/1024;

tauA = 0;
tauB = 0;
tauC = 0;
tauC = 0;
maxValue = 0;
maxAngle = 0;

nbrofvalues = 1000;
tempAngleVector = linspace(0,2*pi,nbrofvalues);

for i = 1:nbrofvalues
    tempAngle = tempAngleVector(i);
    
    timedifferenceA = cos(tempAngle)*d/v;     %Top horizontal pair
    timedifferenceB = sin(tempAngle)*d/v;     %Right vertical pair
    timedifferenceC = cos(tempAngle)*d/v;     %Bottom horizontal pair
    timedifferenceD = sin(tempAngle)*d/v;     %Left vertical pair
    
    tauA = cos(tempAngle)*d/v;     %Top horizontal pair
    tauB = sin(tempAngle)*d/v;     %Right vertical pair
    tauC = cos(tempAngle)*d/v;     %Bottom horizontal pair
    tauD = sin(tempAngle)*d/v;     %Left vertical pair

    value = sum(GA(10:502).*exp(-1*j*w(10:502)'*tauA)) + sum(GB(10:502).*exp(-1*j*w(10:502)'*tauB)) + sum(GC(10:502).*exp(-1*j*w(10:502)'*tauC)) + sum(GD(10:502).*exp(-1*j*w(10:502)'*tauD));
    
    if (value >= maxValue),
        maxValue = value;
        maxAngle = tempAngle;
    end
end

maxAngle
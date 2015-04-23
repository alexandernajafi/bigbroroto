
clc
clf
clear all
close all

[z,Fs] = wavread('sound');
x = z(1:1024);
v = 330;                        %Speed of sound [m/s]
phi = 3.74;                      %XY-plane angle [rad]
d = 0.04;                       %Distance between microphones [m]
numberOfPackages = 10;

dtA = cos(phi)*d/v;     %Top horizontal pair
dtB = sin(phi)*d/v;     %Right vertical pair
dtC = cos(phi)*d/v;     %Bottom horizontal pair
dtD = sin(phi)*d/v;     %Left vertical pair

dphaseA = dtA * Fs;           %Convert timedifference into fraction of sample frequency
dphaseB = dtB * Fs;
dphaseC = dtC * Fs;
dphaseD = dtD * Fs;

h1 = sinc((-10:10)-dphaseA);    %Apply phasedifference to microphones
h2 = sinc((-10:10)-0);
h3 = sinc((-10:10)-dphaseB-dphaseC);
h4 = sinc((-10:10)-dphaseD);

x1 = filter(h1,1,(x));            %Apply signal to microphones
x2 = filter(h2,1,(x));
x3 = filter(h3,1,(x));
x4 = filter(h4,1,(x));

dlmwrite('mic1.txt',x1,'precision','%10.10f')
dlmwrite('mic2.txt',x2,'precision','%10.10f')
dlmwrite('mic3.txt',x3,'precision','%10.10f')
dlmwrite('mic4.txt',x4,'precision','%10.10f')

%save('mic1.txt','x1','-ascii','-double')
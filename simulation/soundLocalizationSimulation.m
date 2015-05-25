clf
clear all
close all
clc

sinc = @(x) sin(pi*x+1E-8)./(pi*x + 1E-8);
[z,Fs] = wavread('sound');

sound(z,Fs); %%ffftw-bibliotek
buffersize = 1024;
y = buffer(z, buffersize);

%Fs = 8000;                      %Sample Frequency [Hz]
v = 330;                        %Speed of sound [m/s]
phi = 3.74;                      %XY-plane angle [rad]
d = 0.04;                       %Distance between microphones [m]
numberOfPackages = 10;

%       h1              h2
%          Microphones
%       h3              h4

%Simulation of sound to microphone below

                        %Time difference between microphones
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

%x = randn(1024,1);              %Random noise

%Begin for loop
results = [];
limit = floor(length(y(1,:)/numberOfPackages))-10;
for i = 1:numberOfPackages:limit
    
    GAmean = 0;
    GBmean = 0;
    GCmean = 0;
    GDmean = 0;
    
    
    for n = 1:numberOfPackages
        if (i == limit && n == numberOfPackages)
            
        end
        x = y(:,i+n);
        x1 = filter(h1,1,(x));            %Apply signal to microphones
        x2 = filter(h2,1,(x)); 
        x3 = filter(h3,1,(x));
        x4 = filter(h4,1,(x));

        X1 = fft(x1);                   %Fouriertransform of microphones
        X2 = fft(x2);
        X3 = fft(x3);
        X4 = fft(x4);
        %Cross spectrum calculation
        GA = X2.*conj(X1);
        GB = X2.*conj(X4);
        GC = X4.*conj(X3);
        GD = X1.*conj(X3);
        
        GAmean = (GAmean*(n-1)+GA)/n;
        GBmean = (GBmean*(n-1)+GB)/n;
        GCmean = (GCmean*(n-1)+GC)/n;
        GDmean = (GDmean*(n-1)+GD)/n;
        
    end
        
    w = (0:1023)*2*pi/buffersize;

    %Medelvärdesbilda här. Bufferfunktionen.

    maxValuePhi = 0;
    maxAnglePhi = 0;

    nbrofvalues = 1000;
    tempAngleVector = linspace(0,2*pi,nbrofvalues);

    for i = 1:nbrofvalues
        tempAngle = tempAngleVector(i);
        len = (buffersize/2-10);
        pairA = sum(GAmean(10:len).*exp(-1*j*w(10:len)'*cos(tempAngle)*Fs*d/v)); %Top horizontal pair
        pairB = sum(GBmean(10:len).*exp(-1*j*w(10:len)'*sin(tempAngle)*Fs*d/v)); %Right vertical pair
        pairC = sum(GCmean(10:len).*exp(-1*j*w(10:len)'*cos(tempAngle)*Fs*d/v)); %Bottom horizontal pair
        pairD = sum(GDmean(10:len).*exp(-1*j*w(10:len)'*sin(tempAngle)*Fs*d/v)); %Left vertical pair

        value = pairA + pairB + pairC + pairD;

        if (value >= maxValuePhi),
            maxValuePhi = value;
            maxAnglePhi = tempAngle;
        end
    end
    
    maxAnglePhi
    results = [results maxAnglePhi];
end

plot(results,'o')
hold on
xc = linspace(0,length(results));
plot([0, length(results)],[phi phi])


clf
clear all
close all
clc

sinc = @(x) sin(pi*x+1E-8)./(pi*x + 1E-8);

Fs = 8000;                      %Sample Frequency [Hz]
v = 330;                        %Speed of sound [m/s]
azimutAngle = 0.1;               %Azimut angle [rad]
d = 0.04;                       %Distance between microphones [m]
maxFreq = 4000;                 %Highest allowed Freqency [Hz]

dtA = cos(azimutAngle)*d/v      %Time difference between microphones in top horizontal pair
dtB = sin(azimutAngle)*d/v      %Time difference between microphones in right vertical pair
dtC = cos(azimutAngle)*d/v      %Time difference between microphones in bottom horizontal pair
dtD = sin(azimutAngle)*d/v      %Time difference between microphones in left vertical pair

dphaseA = dtA/(1/Fs);           %Convert timedifference into fraction of sample frequency
dphaseB = dtB/(1/Fs);           %Convert timedifference into fraction of sample frequency
dphaseC = dtC/(1/Fs);           %Convert timedifference into fraction of sample frequency
dphaseD = dtD/(1/Fs);           %Convert timedifference into fraction of sample frequency

% h1a = sinc((-10:10)-dphaseA);   %Microphone 1 in pair A
% h1d = sinc((-10:10)-0);         %Microphone 1 in pair D
% h2ab = sinc((-10:10)+0);        %Microphone 2 in pair A and B
% h3c = sinc((-10:10)-dphaseC);   %Mircophone 3 in pair C
% h3d = sinc((-10:10)-dphaseD);   %Mircophone 3 in pair D
% h4b = sinc((-10:10)-dphaseB);   %Microphone 4 in pair B 
% h4c = sinc((-10:10)-0);         %Mircophone 4 in pair C

h1 = sinc((-10:10)-dphaseA);
h2 = sinc((-10:10)-0);
h3 = sinc((-10:10)-dphaseB-dphaseC);
h4 = sinc((-10:10)-dphaseD);

x = randn(1024,1);              %Random noise

% x1a = filter(h1a, 1, x);        
% x1d = filter(h1d, 1, x);
% x2ab = filter(h2ab, 1, x);
% x3c = filter(h3c, 1, x);
% x3d = filter(h3d, 1, x);
% x4b = filter(h4b, 1, x);
% x4c = filter(h4c, 1, x);

x1 = filter(h1,1,x);
x2 = filter(h2,1,x);
x3 = filter(h3,1,x);
x4 = filter(h4,1,x);

X1 = fft(x1);
X2 = fft(x2);
X3 = fft(x3);
X4 = fft(x4);
 
% X1a = fft(x1a);
% X1d = fft(x1d);
% X2ab = fft(x2ab);
% X3c = fft(x3c);
% X4b = fft(x4b);
% X4c = fft(x4c);

GA = X2.*conj(X1);
GB = X2.*conj(X4);
GC = X4.*conj(X3);
GD = X1.*conj(X3);

% GA = X2ab .* conj(X1a);
% GB = X2ab .* conj(X4b);
% GC = X4c .* conj(X3c);
% GD = 
w = (0:1023)*2*pi/1024;
 
maxvalueA = 0;
maxtauA = 0;
maxvalueB = 0;
maxtauB = 0;
maxvalueC = 0;
maxtauC = 0;
maxvalueD = 0;
maxtauC = 0;

nbrofvalues = 1000;
taus = linspace(-1,1,nbrofvalues);

for i = 1:nbrofvalues  
    value1 = sum(GA(10:502).*exp(-1*j*w(10:502)'*taus(i)));
    value2 = sum(GB(10:502).*exp(-1*j*w(10:502)'*taus(i)));
    value3 = sum(GC(10:502).*exp(-1*j*w(10:502)'*taus(i)));
    value4 = sum(GD(10:502).*exp(-1*j*w(10:502)'*taus(i)));

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
    
    if (value4 >= maxvalueD),
        maxvalueD = value4;
        maxtauD = taus(i); 
    end  
    
end

dtAM = maxtauA*(1/Fs)
dtBM = maxtauB*(1/Fs)
dtCM = maxtauC*(1/Fs)
dtDM = maxtauD*(1/Fs)

%dtA = cos(azimutAngle)*d/v;      %Time difference between microphones in top horizontal pair
%dtB = sin(azimutAngle)*d/v;     %Time difference between microphones in right vertical pair
%dtC = cos(azimutAngle)*d/v;      %Time difference between microphones in bottom horizontal pair
%dtD = sin(azimutAngle)*d/v;     %Time difference between microphones in left vertical pair




if (dtAM > 0 && dtBM > 0)  % Första kvadranten

aziA = acos(dtAM*v/d)
aziB = asin(dtBM*v/d)
aziC = acos(dtCM*v/d)
aziD = asin(dtDM*v/d)   
    
elseif (dtAM < 0 && dtBM > 0) %andra kvardranten

aziA = acos(dtAM*v/d)
aziB = pi - asin(dtBM*v/d) 
aziC = acos(dtCM*v/d)
aziD = pi - asin(dtDM*v/d)     
    
elseif (dtAM < 0 && dtBM < 0) % Tredje kvadranten

aziA = 2*pi - acos(dtAM*v/d)
aziB = pi - asin(dtBM*v/d) 
aziC = 2*pi - acos(dtCM*v/d)
aziD = pi - asin(dtDM*v/d)      
    
elseif (dtAM > 0 && dtBM < 0) %fjärde kvadranten

aziA = 2*pi - acos(dtAM*v/d)
aziB = 2*pi + asin(dtBM*v/d) 
aziC = 2*pi - acos(dtCM*v/d)
aziD = 2*pi + asin(dtDM*v/d)     
      
end

meanAzi = (aziA+aziB+aziC+aziD)/4




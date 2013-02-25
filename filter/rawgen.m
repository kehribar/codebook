%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Arbitrary waveform generation
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
clc;clear all;close all;

F_SAMPLING = 1000; % in Hertz
TIME_LENGTH = 1.024; % in seconds

DATA_POINTS = TIME_LENGTH * F_SAMPLING;
input = zeros(1,DATA_POINTS);

t = (0 : 1/F_SAMPLING : (TIME_LENGTH-(1/F_SAMPLING)));
f = [40,100,400.5];
c = [1,0.3,0.1];

for k = 1:length(f)
    input = input + (c(k) * sin(2*pi*f(k)*t));
end

input = input - min(input);
input = input * (65535/max(input));
input = uint16(input);
figure;plot(input);

fd = fopen('raw.txt','w');
for k = 1:DATA_POINTS
    temp = sprintf('%d\n',input(k));
    fprintf(fd,'%s',temp);
end
fclose(fd);

Fs = F_SAMPLING;
NFFT = 2^nextpow2(DATA_POINTS);
Y = fft(input,NFFT)/DATA_POINTS;
f = Fs/2*linspace(0,1,NFFT/2+1);

% Plot single-sided amplitude spectrum.
figure; plot(f,2*abs(Y(1:NFFT/2+1))) 
title('Single-Sided Amplitude Spectrum of y(t)')
xlabel('Frequency (Hz)')
ylabel('|Y(f)|')

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Arbitrary waveform generation
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
clc;clear all;close all;

F_SAMPLING = 1000; % in Hertz
TIME_LENGTH = 0.8; % in seconds

DATA_POINTS = TIME_LENGTH * F_SAMPLING;
input = zeros(1,DATA_POINTS);

t = (0 : 1/F_SAMPLING : (TIME_LENGTH-(1/F_SAMPLING)));
f = [4,300,400.5];
c = [1,0.1,0.1];

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

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
figure;
plot(output(254:length(output)));
hold;
plot(input(254:length(input)),'r');



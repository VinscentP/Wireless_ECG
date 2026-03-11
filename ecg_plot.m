clc; clear; close all;

% ---- Load CSV ----
% CSV format:
% Lead1,Lead2,Lead3
filename = '/Users/vincentpham/Desktop/Senior_Capstone/r_indices.csv';
data = readmatrix(filename);
Lead1 = data(:,1);
Lead2 = data(:,2);
Lead3 = data(:,3);

% ---- Sampling frequency ----
Fs = 1250;   % Hz (adjust if needed)
t = (0:length(Lead1)-1)/Fs;

% ---- Window to display ----
start_seconds = 2;   % skip the first 2 seconds
end_seconds = 5;     % stop at 5 seconds (so shows 3 seconds worth)

% ---- Plot ----
figure;
subplot(3,1,1)
plot(t, Lead1, 'LineWidth', 1.2)
grid on
title('Original ECG Amplitude – Lead 1')
ylabel('Amplitude (mV)')
xlim([start_seconds end_seconds])

subplot(3,1,2)
plot(t, Lead2, 'LineWidth', 1.2)
grid on
title('Original ECG Amplitude – Lead 2')
ylabel('Amplitude (mV)')
xlim([start_seconds end_seconds])

subplot(3,1,3)
plot(t, Lead3, 'LineWidth', 1.2)
grid on
title('Original ECG Amplitude – Lead 3')
xlabel('Time (seconds)')
ylabel('Amplitude (mV)')
xlim([start_seconds end_seconds])
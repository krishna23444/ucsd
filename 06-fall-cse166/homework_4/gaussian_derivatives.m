% Author: <njoffe@ucsd.edu> Nitay Joffe
% Date: 10/24/2006
% Class: CSE 166 - Image Processing
% Homework: 1
% Problem: 2 - Gaussian derivatives vs. Gabor functions
clear;

% Gaussian derivatives vs. Gabor functions
% (a) Evaluate the 1D Gaussian function e^{-x^2/2\sigma^2} on the interval
%     x: [-16, 15] using \sigma = 3. Compute its approximate third derivative
%     by repeated application of the centered first difference kernel. Plot the
%     resulting function and its DFT magnitude.

x = -16:15;
sigma = 3;
y = exp(-x.^2/(2*sigma^2));
centered_first_difference_kernel = [1 0 -1];

first_derivative = conv(centered_first_difference_kernel, y);
second_derivative = conv(centered_first_difference_kernel, first_derivative);
third_derivative = conv(centered_first_difference_kernel, second_derivative);
third_derivative_frequency_magnitude = abs(fft(third_derivative));

figure;
subplot(1,2,1);
plot(third_derivative);
title('third derivative of 1D gaussian');
subplot(1,2,2);
plot(third_derivative_frequency_magnitude);
title('discrete fourier transform magnitude');

% (b) Experimentally find parameter values for an even or odd 1D Gabor function
%     that closely matches the Gaussian 3rd derivative. Note: you may also need
%     to apply an amplitude scaling factor. Once you have found a good match,
%     make plots showing the quality of the match in both the spatial domain and
%     the frequency domain.

x = -16:15;
sigma = 4;
u_o = 1.1;
amplitude_scaling_factor = 0.4;

gabor = amplitude_scaling_factor * exp(-x.^2 / (2*sigma^2)).*exp(j*2*pi*u_o*x);
gabor_odd = imag(gabor);
gabor_odd_frequency_magnitude = abs(fft(gabor_odd));

figure;
subplot(1,2,1);
hold on;
plot(gabor_odd,'r');
plot(third_derivative);
title('gabor odd overlay with gaussian');
subplot(1,2,2);
hold on;
plot(gabor_odd_frequency_magnitude,'r');
plot(third_derivative_frequency_magnitude);
title('gabor odd frequency magnitude overlay with gaussian');
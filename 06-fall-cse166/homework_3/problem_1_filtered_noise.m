% Author: <njoffe@ucsd.edu> Nitay Joffe
% Date: 10/19/2006
% Class: CSE 166 - Image Processing
% Homework: 3
% Problem: 1 - Filtered Noise
clear;

% (a) Consider the filter with Fourier transform H(u,v) = 1/(u^2+v^2) on the
%     interval [-127, 128] x [-127, 128]. This is known as a 1/f^2 transfer
%     function. Since it blows up at the origin, replace that value with zero.
%     Apply this filter to a 256x256 image of normally distributed random
%     noise (use _randn_). For practical reasons, it is best to perform this
%     operation in the frequency domain. Hint: you will need to use _meshgrid_,
%     _fft2_, _ifft2_, and _fftshift_. Also, due to numerical error, you will
%     need to use _real_ to look at the real part of the filtered image in the
%     spatial domain.
u_range = -127:128;
v_range = u_range;

% Evaluate filter function over a grid of values.
[u,v] = meshgrid(u_range,v_range);
filter_kernel_frequency = 1 ./ (u.^2 + v.^2);
filter_kernel_frequency(filter_kernel_frequency == inf) = 0;

% Generate image of normally distributed random noise.
noise_image = randn(256,256);
noise_image_frequency = fft2(noise_image);

% Run filter on noise image.
filtered_noise_image_frequency = filter_kernel_frequency.*noise_image_frequency;
filtered_noise_image_frequency_shifted=fftshift(filtered_noise_image_frequency);

% Use inverse fourier transform to get filtered noise image in spatial domain.
filtered_noise_image = ifft2(filtered_noise_image_frequency_shifted);
filtered_noise_image_real = real(filtered_noise_image);

% (b) Display the filtered image along with the original noise image. Quite
%     remarkably, the filtered image should look like a "natural" texture, such
%     as clouds or terrain. What does this suggest about the statistics of
%     natural images vs. that of images of manmade objects?
figure;
subplot(1,2,1);
imagesc(noise_image);
title('original normally distributed noise image');
subplot(1,2,2);
imagesc(filtered_noise_image_real);
title('filtered image');

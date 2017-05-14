% Author: <njoffe@ucsd.edu> Nitay Joffe
% Date: 10/12/2006
% Class: CSE 166 - Image Processing
% Homework: 2
% Problem: 5 - Computing 2D Discrete Fourier Transforms
clear;

% (a) Compute and display the magnitude of the DFT of Figures 4.3(a) and
%     4.4(a). The Matlab function for the 2D DFT is fft2. Use fftshift to move
%     the DC component to the center, and use the logarithmic transformation so
%     that your results look like Figures 4.3(b) and 4.4(b), respectively. In
%     both cases, use imagesc with CLIM=[5 13] to display the DFT log
%     magnitude.
clim = [5 13];
I1 = double(imread('Fig4.03(a).jpg'));
I2 = double(imread('Fig4.04(a).jpg'));
F1_shifted_log_magnitude = log(abs(fftshift(fft2(I1))));
F2_shifted_log_magnitude = log(abs(fftshift(fft2(I2))));

figure;
subplot(1,2,1);
imagesc(F1_shifted_log_magnitude, clim);
title('DFT of Figure 4.3(a)');
subplot(1,2,2);
imagesc(F2_shifted_log_magnitude, clim);
title('shifted log magnitude of DFT of Figure 4.4(a)');

% (b) Compute and display the DFT magnitude (again using the logarithmic
%     transformation) for an image of your own choosing, e.g. taken from the
%     internet or your own digital camera. If the image is color, use rgb2gray
%     to convert it to monochrome.
I = double(rgb2gray(imread('funny-pictures-cat.jpg')));
F_shifted_log_magnitude = log(abs(fftshift(fft2(I))));

figure;
subplot(1,2,1);
imshow('funny-pictures-cat.jpg');
title('original image');
subplot(1,2,2);
imagesc(F_shifted_log_magnitude, clim);
title('shifted log magnitude of DFT of image');
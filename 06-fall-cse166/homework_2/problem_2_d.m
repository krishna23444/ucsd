% Author: <njoffe@ucsd.edu> Nitay Joffe
% Date: 10/12/2006
% Class: CSE 166 - Image Processing
% Homework: 2
% Problem: 2 - Canny Edge Detection
% Section: d
clear;

% (d) Apply your edge detector to Figure 1.14(c), adjusting sigma and tau as
%     you see fit. Display the resulting edges and the parameter settings used.

I = double(imread('Fig1.14(c).jpg'));

sigma = 1;
tau = 7;

[E,M,A] = canny(I,sigma,tau);

E = ~E;

figure;
imshow(E);
title(['edges detected using canny with sigma = ' num2str(sigma) ', and tau = ' num2str(tau)]);
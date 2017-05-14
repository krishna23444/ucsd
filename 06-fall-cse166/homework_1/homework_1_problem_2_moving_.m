% Author: <njoffe@ucsd.edu> (Nitay Joffe)
% Date: 10/03/2006
% Class: CSE 166 - Image Processing
% Homework: 1
% Problem: 2 - Moving Averages

% Part (a).
h_box = [1 1 1]/3;

% Create a vector x of length 100 representing a step edge.
x = [zeros(1,50) ones(1,50)];

% Add Gaussian noise with standard deviation 0.05.
x = x + 0.05*randn(1,100);

% Convolve x with h_box.
x_h_box = conv(x,h_box);

% Part (c).
% Set h_pasc equal to the fifth row  of Pascal's triangle.
h_pasc = [1 4 6 4 1];

% Normalize it so that it sums to 1.
h_pasc = h_pasc ./ sum(h_pasc);

x_h_pasc = conv(x,h_pasc);

% Part (d)
% Set x2 equal to row 375 of Figure 3.35(a).
figure_335a = double(imread('Fig3.35(a).jpg'));
x2 = figure_335a(375);

% Convolve this signal with each of hte above kernels and plot the results.
x2_h_box = conv(x2,h_box);
x2_h_pasc = conv(x2,h_pasc);

% Plot results on one page.
subplot(4,2,1);
stem(x);
title('step edge');

subplot(4,2,2);
stem(x2);
title('row 375 of figure 3.35(a)');

subplot(4,2,3);
stem(x_h_box);
title('step edge after convolution with box filter');

subplot(4,2,4);
stem(x2_h_box);
title('row 375 of figure 3.35(a) convolved with box filter');

subplot(4,2,5);
stem(x_h_pasc);
title('step edge after convolution with binomial lowpass filter');

subplot(4,2,6);
stem(x2_h_pasc);
title('row 375 of figure 3.35(a) convolved with binomial lowpass filter');

subplot(4,2,7);
stem(h_box);
title('box filter');

subplot(4,2,8);
stem(h_pasc);
title('binomial lowpass filter');

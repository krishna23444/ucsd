% Author: <njoffe@ucsd.edu> Nitay Joffe
% Date: 10/12/2006
% Class: CSE 166 - Image Processing
% Homework: 2
% Problem: 4 - Computing 1D Discrete Fourier Transforms
clear;

% Compute and display the DFT of the following 1D signals. The Matlab function
% for the 1D DFT is fft. For each DFT, use stem to display the following three
% plots: (1) original signal, (2) DFT magnitude (using abs), (3) DFT phase
% (using angle). In the latter two plots, use fftshift to place the DC
% component at the middle. Label the axes and put a title on each plot. Set the
% y axis range on the phase plots to [-pi, pi].
phase_plot_y_axis_range = [-pi, pi];

% (a) delta(x - xo) for x = 0, 1, . . . , 7 with the following values of
%     xo : 0, 1, 4.
x_range = 0:7;
x_0_values = [0 1 4];
number_of_items = numel(x_0_values);

figure;
for i = 1:number_of_items
  x_0 = x_0_values(i);
  f = dirac(x_range - x_0) > 0;
  F = fft(f);
  F_magnitude_shifted = fftshift(abs(F));
  F_angle_shifted = fftshift(angle(F));

  subplot(number_of_items,3,i*3-2);
  stem(x_range, f);
  xlabel('x');
  ylabel('f(x)');
  title(['original signal, x_0 = ' num2str(x_0)]);

  subplot(number_of_items,3,i*3-1);
  stem(F_magnitude_shifted);
  xlabel('u - frequency');
  ylabel('magnitude');
  title(['DFT magnitude, x_0 = ' num2str(x_0)]);

  subplot(number_of_items,3,i*3);
  stem(F_angle_shifted);
  ylim(phase_plot_y_axis_range);
  xlabel('u - frequency');
  ylabel('phase - radians');
  title(['DFT phase, x_0 = ' num2str(x_0)]);
end

% (b) cos w_o x for x = 0, 1, . . . , 7 with the following values of
%     w_o : 0, pi/4, pi/2, pi.
x_range = 0:7;
w_0_values = [0, pi/4, pi/2, pi];
number_of_items = numel(w_0_values);

figure;
for i = 1:number_of_items
  w_0 = w_0_values(i);
  f = cos(x_range .* w_0);

  F = fft(f);
  F_magnitude_shifted = fftshift(abs(F));
  F_angle_shifted = fftshift(angle(F));

  subplot(number_of_items,3,i*3-2);
  stem(x_range, f);
  xlabel('x');
  ylabel('f(x)');
  title(['original signal, w_0 = ' num2str(w_0)]);
  
  subplot(number_of_items,3,i*3-1);
  stem(F_magnitude_shifted);
  xlabel('u - frequency');
  ylabel('magnitude');
  title(['DFT magnitude, w_0 = ' num2str(w_0)]);
  
  subplot(number_of_items,3,i*3);
  stem(F_angle_shifted);
  ylim(phase_plot_y_axis_range);
  xlabel('u - frequency');
  ylabel('phase - radians');
  title(['DFT phase, w_0 = ' num2str(w_0)]);
end

% (c) The first difference kernel, padded with zeros to
%     length 8: [0.5 0 -0.5 0 0 0 0 0].
f = [0.5 0 -0.5 0 0 0 0 0];
F = fft(f);
F_magnitude_shifted = fftshift(abs(F));
F_angle_shifted = fftshift(angle(F));

figure;
subplot(1,3,1);
stem(0:7, f);
xlabel('x');
ylabel('f(x)');
title('original signal');

subplot(1,3,2);
stem(F_magnitude_shifted);
xlabel('u - frequency');
ylabel('magnitude');
title('DFT magnitude');

subplot(1,3,3);
stem(F_angle_shifted);
ylim(phase_plot_y_axis_range);
xlabel('u - frequency');
ylabel('phase - radians');
title('DFT phase0');

% (d) The binomial kernel with the following values of ?^2 : 1, 2, 3. In each
%     case, pad the kernel with zeros to make the total length 64.
length = 64;
sigma_squared_values = [1 2 3];
number_of_items = numel(sigma_squared_values);

figure;
for i = 1:number_of_items
  sigma_squared = sigma_squared_values(i);
  pascal_row_number = 4*sigma_squared+1;
  pascal_row = diag(rot90(pascal(pascal_row_number)))';
  normalized_pascal_row = pascal_row ./ sum(pascal_row);
  f = [normalized_pascal_row, zeros(1,length-pascal_row_number)];

  F = fft(f);
  F_magnitude_shifted = fftshift(abs(F));
  F_angle_shifted = fftshift(angle(F));

  subplot(number_of_items,3,i*3-2);
  stem(f);
  xlabel('x');
  ylabel('f(x)');
  title(['original signal, sigma^2 = ' num2str(sigma_squared)]);
  
  subplot(number_of_items,3,i*3-1);
  stem(F_magnitude_shifted);
  xlabel('u - frequency');
  ylabel('magnitude');
  title(['DFT magnitude, sigma^2 = ' num2str(sigma_squared)]);
  
  subplot(number_of_items,3,i*3);
  stem(F_angle_shifted);
  ylim(phase_plot_y_axis_range);
  xlabel('u - frequency');
  ylabel('phase - radians');
  title(['DFT phase, sigma^2 = ' num2str(sigma_squared)]);
end
% Author: <njoffe@ucsd.edu> Nitay Joffe
% Date: 10/24/2006
% Class: CSE 166 - Image Processing
% Homework: 1
% Problem: 1 - Gabor Functions
clear;

% 1. Gabor Functions
%   In 1D, the Gabor function of width sigma and spatial frequency u_o is given
%   by the following complex-valued function:
%       h(x) = e^{-x^2 / 2\sigma^2} e^{j2\pi u_o x}
%   In 2D, the corresponding expression is
%       h(x) = e^{-||X||^2 / 2\sigma^2} e^{j2\pi U_o X}
%   where X = (x, y) and U_o = (u_o , v_o), and it serves as an oriented
%   bandpass filter. The even and odd Gabor functions are equal to the real and
%   imaginary parts of h, respectively.

% (a) Compute eight examples of even and/or odd 1D Gabor functions on the
%     interval x: [-16, 15] using parameters chosen in the following ranges:
%     sigma: [1, 3] and u_o: [0, 0.3]. For each example, plot the function and
%     its Fourier transform magnitude.

x = -16:15;
sigma_values = [1.5 2.75];
u_0_values = [0.0 0.1 0.2 0.3];
number_of_items = numel(sigma_values) * numel(u_0_values);

plot_even_function = true;
i = 1;

figure;
for sigma = sigma_values
  for u_0 = u_0_values
    gabor_function = exp(-x.*x/(2*sigma^2)).*exp(j*2*pi*u_0*x);
    even_gabor_function = real(gabor_function);
    odd_gabor_function = imag(gabor_function);

    gabor_function_frequency = fft(gabor_function);
    even_gabor_function_frequency_magnitude = abs(fft(even_gabor_function));
    odd_gabor_function_frequency_magnitude = abs(fft(odd_gabor_function));
    
    subplot(number_of_items,2,i);
    if plot_even_function
      plot(even_gabor_function);
      title(['even gabor function with sigma = ' num2str(sigma) ', u_0 = ' num2str(u_0)]);
    else
      plot(odd_gabor_function);
      title(['odd gabor function with sigma = ' num2str(sigma) ', u_0 = ' num2str(u_0)]);
    end

    subplot(number_of_items,2,i+1);
    if plot_even_function
      plot(even_gabor_function_frequency_magnitude);
      title(['even gabor function frequency with sigma = ' num2str(sigma) ', u_0 = ' num2str(u_0)]);
    else
      plot(odd_gabor_function_frequency_magnitude);
      title(['odd gabor function frequency magnitude with sigma = ' num2str(sigma) ', u_0 = ' num2str(u_0)]);
    end
    i = i + 2;
  end
end

% (b) Compute eight examples of even and/or odd 2D Gabor functions on the
%     interval x: [-16, 15]x[-16, 15] using parameters chosen in the following
%     ranges: \sigma: [1, 3] and u_o = [0, 0.3]x[0, 0.3]. For each example,
%     display the function (either as an image or a surface plot) and its
%     Fourier transform magnitude.

[x,y] = meshgrid(-16:15,-16:15);
sigma_values = [1.75 2.5];
u_0_values = [0 0; 0.1 0.3; 0.3 0.2; 0.2 0.05];
number_of_u_0_rows = size(u_0_values,1);

plot_even_function = true;

for sigma = sigma_values
  for u_0_row_index = 1:number_of_u_0_rows
    u_0 = u_0_values(u_0_row_index,1);
    v_0 = u_0_values(u_0_row_index,2);

    u_0_x_dot_product = u_0 * x + v_0 * y;
    gabor_function = exp(-abs(x.*x)/(2*sigma^2)).*exp(j*2*pi*u_0_x_dot_product);
    even_gabor_function = real(gabor_function);
    odd_gabor_function = imag(gabor_function);

    gabor_function_frequency = fft2(gabor_function);
    even_gabor_function_frequency_magnitude = abs(fft2(even_gabor_function));
    odd_gabor_function_frequency_magnitude = abs(fft2(odd_gabor_function));

    figure;
    subplot(1,2,1);
    if plot_even_function
      mesh(x,y,even_gabor_function);
      title(['even gabor function with \sigma = ' num2str(sigma) ', u_0 = ' num2str(u_0), ', v_0 = ' num2str(v_0)]);
    else
      mesh(x,y,odd_gabor_function);
      title(['odd gabor function with \sigma = ' num2str(sigma) ', u_0 = ' num2str(u_0), ', v_0 = ' num2str(v_0)]);
    end

    subplot(1,2,2);
    if plot_even_function
      mesh(x,y,even_gabor_function_frequency_magnitude);
      title(['even gabor function frequency with \sigma = ' num2str(sigma) ', u_0 = ' num2str(u_0), ', v_0 = ' num2str(v_0)]);
    else
      mesh(x,y,odd_gabor_function_frequency_magnitude);
      title(['odd gabor function frequency magnitude with \sigma = ' num2str(sigma) ', u_0 = ' num2str(u_0), ', v_0 = ' num2str(v_0)]);
    end
  end
end

% (c) Load in the fingerprint image of Figure 10.29(a) and visually inspect it
%     to determine the approximate spacing between the ridges. Choose two 2D
%     Gabor functions, one even and one odd, with ||u_o|| approximately equal to the
%     ridge frequency. Display the raw fingerprint image, the even filter, and
%     the odd filter along with the magnitudes of their respective Fourier
%     transforms. Note: zero-pad the filters to the size of the image before
%     transforming them, and use the logarithmic transformation to display the
%     transform magnitude. Now filter the fingerprint image with each Gabor
%     function and display the results. Explain what you observe.

% image_1029a = imread('Fig10.29(a).jpg');
% 
% [x,y] = size(image_1029a);
% imshow(image_1029a);
% 
% u_0 = [1.0 1.0];
% x = meshgrid(x_range,y_range);
% 
% gabor_function = exp(-abs(x.*x)/(2*sigma^2)).*exp(j*2*pi*u_0_x_dot_product);
% even_gabor_function = real(gabor_function);
% odd_gabor_function = imag(gabor_function);
% 
% even_gabor_function_frequency_magnitude = abs(fft2(even_gabor_function));
% odd_gabor_function_frequency_magnitude = abs(fft2(odd_gabor_function));
% 
% image_odd_gabor = conv2(image_1029a, odd_gabor_function, 'valid');
% image_even_gabor = conv2(image_1029a, even_gabor_function, 'valid');
% 
% figure;
% subplot;
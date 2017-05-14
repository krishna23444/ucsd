% Author: <njoffe@ucsd.edu> Nitay Joffe
% Date: 10/19/2006
% Class: CSE 166 - Image Processing
% Homework: 3
% Problem: 2 - Filtering in the Frequency Domain
clear;

% (a) Write an m-file to reproduce Figure 4.41(a-f) using Frequency domain
%     filtering. Note: the Matlab command for the complex conjugate is conj.
original_image = double(imread('Fig4.41(a).jpg'));
template_image = double(imread('Fig4.41(b).jpg'));

% Compute minimum size of images padded with zeros.
[image_x_size, image_y_size] = size(original_image);
[template_x_size, template_y_size] = size(template_image);
padded_image_x_size = image_x_size + template_x_size - 1;
padded_image_y_size = image_y_size + template_y_size - 1;

% Pad images with zeros so they are the same size and frequencies don't corrupt
% the fourier transform computations.
image_padded = zeros(padded_image_x_size, padded_image_y_size);
image_padded(1:image_x_size,1:image_y_size) = original_image;
template_padded = zeros(padded_image_x_size, padded_image_y_size);
template_padded(1:template_x_size,1:template_y_size) = template_image;

% Run discrete fourier transform on images to get frequencies.
image_padded_frequency = fft2(image_padded);
template_padded_frequency = fft2(template_padded);

% Compute conjugate of template. Use it to get correlation in frequency domain.
template_padded_frequency_conjugate = conj(template_padded_frequency);
correlation_image_frequency = template_padded_frequency_conjugate.*image_padded_frequency;
correlation_image_real = real(ifft2(correlation_image_frequency));

% Find row containing highest value in correlation image, get profile of it.
[max_per_column, max_per_column_indices] = max(correlation_image_real);
[max_overall, max_overall_index_in_columns] = max(max_per_column);
highest_value_row = max_per_column_indices(max_overall_index_in_columns);
profile_highest_value_along_x = correlation_image_real(highest_value_row,:);

% Display results.
figure;
subplot(3,2,1);
imshow(original_image);
title('original image');
subplot(3,2,2);
imshow(template_image);
title('template image');
subplot(3,2,3);
imshow(image_padded);
title('original image padded with zeros');
subplot(3,2,4);
imshow(template_padded);
title('template image padded with zeros');
subplot(3,2,5);
imagesc(correlation_image_real);
% Set aspect ratio so that x and y increments are equal, and make plot box fit
% tightly around the data.
axis image;
title('correlation image');
subplot(3,2,6);
plot(profile_highest_value_along_x);
title('profile along x containing highest correlation value');

% (b) Repeat the previous step using operations in the spatial domain and show
%     that the results are the same. Hint: use conv2 and rot90.

%----- IMPORTANT -----
% NOTE: The code for reading and padding the image in the spatial domain is
%       _exactly_ the same, so rather than copying it I just used the results.

% Compute conjugate of original image for spatial correlation computation.
image_conjugate = conj(original_image);

% Need to rotate the template image 180 degrees since correlation does not flip
% the image. This is to undo the 180 degrees flip that convolution does.
template_rotated = rot90(rot90(template_image));

% Compute spatial correlation between images using 2D convolution.
correlation_image = conv2(image_conjugate, template_rotated, 'valid');

% Find row containing highest value in correlation image, get profile of it.
[max_per_column, max_per_column_indices] = max(correlation_image);
[max_overall, max_overall_index_in_columns] = max(max_per_column);
highest_value_row = max_per_column_indices(max_overall_index_in_columns);
profile_highest_value_along_x = correlation_image_real(highest_value_row,:);

% Display results.
figure;
subplot(3,2,1);
imshow(original_image);
title('original image');
subplot(3,2,2);
imshow(template_image);
title('template image');
subplot(3,2,3);
imshow(image_padded);
title('original image padded with zeros');
subplot(3,2,4);
imshow(template_padded);
title('template image padded with zeros');
subplot(3,2,5);
imagesc(correlation_image);
% Set aspect ratio so that x and y increments are equal, and make plot box fit
% tightly around the data.
axis image;
title('correlation image');
subplot(3,2,6);
plot(profile_highest_value_along_x);
title('profile along x containing highest correlation value');
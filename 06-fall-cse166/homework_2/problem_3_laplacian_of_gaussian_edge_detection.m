% Author: <njoffe@ucsd.edu> Nitay Joffe
% Date: 10/12/2006
% Class: CSE 166 - Image Processing
% Homework: 2
% Problem: 3 - Laplacian of Gaussian Edge Detection
clear;

% a) Use fspecial with the 'LoG' option to construct a 15x15 isotropic
%    Laplacian of Gaussian kernel with SIGMA=2. Call the kernel H. Make a mesh
%    plot of -H.
hsize = 15;
sigma = 2;
H = fspecial('LoG', hsize, sigma);

figure;
mesh(-H);
title('mesh plot of LoG kernel H');

% b) Using conv2 with the 'same' option, apply H to Figure 10.15(a) and call the
%    result F. Display the original image, the filtered image, and the filtered
%    image thresholded at zero.
I = imread('Fig10.15(a).jpg');
I_double = double(I);
F = conv2(I_double, H, 'same');
F_thresholded = F;
F_thresholded(F_thresholded < 0) = 0;

figure;
subplot(1,3,1);
imshow(I);
title('original image');
subplot(1,3,2);
imshow(F);
title('filtered image');
subplot(1,3,3);
imshow(F_thresholded);
title('filtered image thresholded at zero');

% c) Display the zero crossings of F as a contour plot superimposed on the raw
%    image. (Hint: use contour with level set parameter [0 0] and color ?k? or
%    ?r?.)
figure;
imshow(I);
hold;
contour(F, [0 0], 'r');
title('zero crossings of filtered image F');

% d) The contour plot in the previous step displays the zero crossings
%    regardless of edge strength. We can use the gradient magnitude to suppress
%    contours for weak edges as follows. Compute the smoothed gradient
%    magnitude M as in the previous exercise, using ? = 2. Set all pixels in F
%    for which M<tau equal to NaN. Pick a value of ? in the interval [2, 6].
%    Now reproduce the superimposed contour plot with this threshold in place.
sigma = 2;
sigma_squared = sigma^2;
pascal_row_number = 4*sigma_squared + 1;
pascal_row = diag(rot90(pascal(pascal_row_number)));
smoothing_filter = pascal_row ./ sum(pascal_row);
smoothed_I = conv2(smoothing_filter, smoothing_filter, I, 'same');

threshold_tau = 4;
[gradient_x, gradient_y] = gradient(smoothed_I);
gradient_magnitude = sqrt(gradient_x.*gradient_x + gradient_y.*gradient_y);
F(gradient_magnitude < threshold_tau) = NaN;

figure;
imshow(I);
hold;
contour(F, [0 0], 'r');
title(['contour plot after suppressing weak edges with tau = ' num2str(threshold_tau)]);
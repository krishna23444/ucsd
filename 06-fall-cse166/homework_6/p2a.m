% Author: <njoffe@ucsd.edu> Nitay Joffe
% Date: 11/21/2006
% Class: CSE 166 - Image Processing
% Homework: 6
% Problem: 2 - Interest Point Detection
% Part: a
clear;

% (a) Compute the eigenvalues (\lambda_{max},\lambda_{min}) of the Forstner
%     interest operator for the checker-board image in the figure for GW Problem
%     10.18(right). Use a window size of 3 x 3. On top of the original
%     checkerboard image, plot the coordinates (use the '.' pointmarker) of all
%     pixels for which \lambda_{min} > \tau , with \tau set to 80% of the
%     maximum value of \lambda_{min} over the whole image. The resulting
%     coordinates should fall on or near the corners of the squares in the image.
I = double(imread('Prob10.18(right).jpg'));
[gradient_x,gradient_y] = gradient(I);
gradient_x_squared = gradient_x.*gradient_x;
gradient_y_squared = gradient_y.*gradient_y;
gradient_x_times_y = gradient_x.*gradient_y;
window_1d = [1 1 1];
sum_x_squared_window = conv2(window_1d,window_1d,gradient_x_squared,'same');
sum_y_squared_window = conv2(window_1d,window_1d,gradient_y_squared,'same');
sum_x_times_y_window = conv2(window_1d,window_1d,gradient_x_times_y,'same');

[rows,columns] = size(sum_x_times_y_window);
maximum_eigenvalue_small = 0;
eigenvalues_small = [];
for i=1:rows
  for j=1:columns
    C = [sum_x_squared_window(i,j),sum_x_times_y_window(i,j);
         sum_x_times_y_window(i,j),sum_y_squared_window(i,j)];
    small_eigenvalue = min(eig(C));
    eigenvalues_small(i,j) = small_eigenvalue;
    if (small_eigenvalue > maximum_eigenvalue_small)
      maximum_eigenvalue_small = small_eigenvalue;
    end
  end
end
tau_fraction_of_max_eigenvalue_small = 0.8;
tau = tau_fraction_of_max_eigenvalue_small * maximum_eigenvalue_small;
corners = [];
for i=1:rows
  for j=1:columns
    if (eigenvalues_small(i,j) > tau)
      corners(:,end+1) = [i;j];
    end
  end
end
corners_x = corners(1,:);
corners_y = corners(2,:);


figure;
imshow(I);
hold;
plot(corners_x,corners_y,'.');
title('Checkerboard with interesting points');

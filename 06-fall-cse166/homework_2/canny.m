% Author: <njoffe@ucsd.edu> Nitay Joffe
% Date: 10/12/2006
% Class: CSE 166 - Image Processing
% Homework: 2
% Problem: 2 - Canny Edge Detection
% Section: b

% (b) Implement the simpli?ed version of the Canny edge detector as described
%     in lecture. The syntax of your function should be as follows:
%     [E,M,A]=canny(I,sig,tau), where E contains the detected edges, M contains
%     the smoothed gradient magnitude, A contains the gradient angle, I is the
%     input image, sig represents ? for the smoothing ?lter, and tau is the
%     threshold ?. You do not need to implement hysteresis thresholding, but
%     you do need to implement oriented nonmaximal suppression, as described in
%     lecture. For extra credit, implement hysteresis thresholding with edge
%     tracking.
function [E,M,A] = canny(I,sig,tau)
  % [E,M,A] = canny(I,sig,tau)
  %
  % I is the input image.
  % sig represents sigma for the smoothing filter.
  % tau is the threshold.
  %
  % E contains the detected edges.
  % M contains the smoothed gradient magnitude.
  % A contains the gradient angle.  

  % Use discrete binomial approximation of Gaussian. Grab row from pascal's
  % triangle at 4*sigma^2+1 and normalize. 
  pascal_row_number = 4 * sig^2 + 1;
  pascal_row = diag(rot90(pascal(pascal_row_number)))';
  smoothing_kernel = pascal_row ./ sum(pascal_row);

  % Convolve image with the smoothing filter in X and Y directions. Produce an
  % image that is the same size as the original.
  smoothed_image = conv2(smoothing_kernel,smoothing_kernel,double(I),'same');

  % Compute gradient magnitude and angle of smoothed image.
  [gradient_x, gradient_y] = gradient(smoothed_image);
  M = sqrt(gradient_x.*gradient_x + gradient_y.*gradient_y);
  A = atan(gradient_y./gradient_x);

  % Thresholding, zero all pixels with magnitude lower than tau.
  E = M;
  E(E < tau) = 0;

  row_count = size(M, 1);
  column_count = size(M, 2);

  % Oriented nonmaximal suppression.
  % Find X and Y coordinates of all the possible edges, that is all the pixels
  % whose magnitude was not zeroed out during thresholding.
  edge_index = find(E);
  edge_index_x = floor(edge_index ./ column_count);
  edge_index_y = edge_index - edge_index_x .* column_count;

  % Compute edge normal direction and use it to figure out deltas for neighbors
  % around edge pixel.
  edge_direction_angle = A(edge_index);
  edge_normal_angle = edge_direction_angle + pi/2;
  neighbor_x_delta = round(cos(edge_normal_angle));
  neighbor_y_delta = round(sin(edge_normal_angle));

  % Get magnitude of neighboring pixels in direction of edge normal.
  % Avoid errors with indexes out of range.
  neighbor_1_index_x = edge_index_x - neighbor_x_delta;
  neighbor_1_index_x(neighbor_1_index_x < 1) = 1;
  neighbor_1_index_x(neighbor_1_index_x > row_count) = row_count;
  neighbor_1_index_y = edge_index_y - neighbor_y_delta;
  neighbor_1_index_y(neighbor_1_index_y < 1) = 1;
  neighbor_1_index_y(neighbor_1_index_y > column_count) = column_count;
  neighbor_1_indexes = (neighbor_1_index_x-1) .* column_count + neighbor_1_index_y;
  neighbor_1_magnitude = M(neighbor_1_indexes);

  neighbor_2_index_x = edge_index_x + neighbor_x_delta;
  neighbor_2_index_x(neighbor_2_index_x < 1) = 1;
  neighbor_2_index_x(neighbor_2_index_x > row_count) = row_count;
  neighbor_2_index_y = edge_index_y + neighbor_y_delta;
  neighbor_2_index_y(neighbor_2_index_y < 1) = 1;
  neighbor_2_index_y(neighbor_2_index_y > column_count) = column_count;
  neighbor_2_indexes = (neighbor_2_index_x-1) .* column_count + neighbor_2_index_y;
  neighbor_2_magnitude = M(neighbor_2_indexes);

  % Zero out edge pixels that are less than either of their neighbors.
  edge_magnitude = M(edge_index);
  E(edge_magnitude < neighbor_1_magnitude) = 0;
  E(edge_magnitude < neighbor_2_magnitude) = 0;

  % Remaining nonzero magnitude pixels are the edges.
  E = logical(E > 0);

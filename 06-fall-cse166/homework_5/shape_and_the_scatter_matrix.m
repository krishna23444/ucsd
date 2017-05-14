% Author: <njoffe@ucsd.edu> Nitay Joffe
% Date: 11/14/2006
% Class: CSE 166 - Image Processing
% Homework: 5
% Problem: 2 - Shape and the scatter matrix
clear;

% (a) Load in GW Figure 11.10, call it I, and binarize using the command
%     BW=I>128. Use find to obtain the (x, y) coordinates of the nonzero pixels.
%     Plot the resulting pointset using the axis('image') and axis('ij') options
%     and the 'b.' pointmarker.
I = imread('Fig11.10.jpg');
BW = I>128;
[x,y] = find(BW);
number_of_elements = numel(x);

% (b) Compute the centroid m and plot it in the preceding figure (turn hold on)
%     using the 'rx' pointmarker.
m_x = mean(x);
m_y = mean(y);

% (c) Compute and display the scatter matrix C. Find its eigenvalues and
%     eigenvectors, first using the above shortcuts, then using the Matlab
%     function eig, and demonstrate that both methods give you the same result.
x_centered = x - m_x;
y_centered = y - m_y;
C = [sum(x_centered.*x_centered), sum(x_centered.*y_centered);
     sum(y_centered.*x_centered), sum(y_centered.*y_centered)];
C = C./number_of_elements

eigenvalue_1_shortcut = (trace(C)+sqrt(trace(C)^2-4*det(C)))/2
eigenvalue_2_shortcut = (trace(C)-sqrt(trace(C)^2-4*det(C)))/2
eigenvector_1_shortcut_angle = 0.5*atan2(2*C(1,2),(C(1,1)-C(2,2)))
eigenvector_2_shortcut_angle = eigenvector_1_shortcut_angle+pi/2
eigenvector_1 = [-cos(eigenvector_1_shortcut_angle);
                 -sin(eigenvector_1_shortcut_angle)]
eigenvector_2 = [-cos(eigenvector_2_shortcut_angle);
                 -sin(eigenvector_2_shortcut_angle)]

[eigenvectors,eigenvalues] = eig(C)

% (d) Compute the aspect ratio of this shape using the formula
%     (\lambda_max / \lambda_min)^{1/2}.
lambda_max = eigenvalue_1_shortcut;
lambda_min = eigenvalue_2_shortcut;
aspect_ratio = sqrt(lambda_max/lambda_min)

% (e) Center the pointset so that its centroid lies on the origin. By visual
%     inspection, estimate the rotation (in degrees) of the shape with respect
%     to horizontal. Compare this to the estimate of the rotation provided by
%     \phi. Now derotate the coordinates so that the shape is oriented along the
%     x-axis, and make a plot of the result.
points = eigenvectors * [x_centered';y_centered'];
x_rotated = points(1,:);
y_rotated = points(2,:);

figure;
subplot(1,3,1);
plot(x,y,'b.');
hold on;
plot(m_x,m_y,'rx');
axis image;
axis ij;
title('original image with centroid');
subplot(1,3,2);
plot(x_centered,y_centered,'b.');
axis image;
axis ij;
title('centered image');
subplot(1,3,3);
plot(x_rotated, y_rotated, 'b.');
axis image;
axis ij;
title('rotated image');
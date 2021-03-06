% Author: <njoffe@ucsd.edu> Nitay Joffe
% Date: 11/21/2006
% Class: CSE 166 - Image Processing
% Homework: 6
% Problem: 1 - Estimating Affine Transformations
clear;

% (a) Plot the two pointsets on the same set of axes. Use the 'x-'
%     pointmarker/linetype for the first set and 'o-' for the second.
set_1_x = [1.8158,1.8626,3.4883,1.8860,1.8860,3.5234];
set_1_y = [4.0673,2.7924,2.6170,2.4181,1.1667,1.2135];
set_1 = [set_1_x;
         set_1_y];

set_2_x = [1.5468,2.2251,3.9327,2.4942,3.1140,4.4591];
set_2_y = [3.5760,2.0322,2.6170,1.5877,0.5585,1.4123];
set_2 = [set_2_x;
         set_2_y];

% (b) Solve for the least-squares affine transform (consisting of a 2 x 2 matrix
%     A and a 2 x 1 translation vector t) that maps the first pointset onto the
%     second. Display the values you obtain for A and t.
set_1_x_mean = mean(set_1_x)
set_1_y_mean = mean(set_1_y);

set_2_x_mean = mean(set_2_x)
set_2_y_mean = mean(set_2_y);

set_1_normalized_x = set_1_x - set_1_x_mean;
set_1_normalized_y = set_1_y - set_1_y_mean;
set_1_normalized = [set_1_normalized_x;
                    set_1_normalized_y];

set_2_normalized_x = set_2_x - set_2_x_mean;
set_2_normalized_y = set_2_y - set_2_y_mean;
set_2_normalized = [set_2_normalized_x;
                    set_2_normalized_y];

t_x = set_2_x_mean - set_1_x_mean;
t_y = set_2_y_mean - set_1_y_mean;
t = [t_x;
     t_y]
A = set_2_normalized / set_1_normalized;

% (c) Use the estimated affine transform to align the two pointsets, and make a
%     plot to show the alignment. Compute and display the sum of the squared
%     Euclidean distances between the corresponding point pairs before and
%     after alignment.
set_1_projected = A * set_1;
set_1_projected_x = set_1_projected(1,:) + t_x;
set_1_projected_y = set_1_projected(2,:) + t_y;


figure;
subplot(1,2,1);
plot(set_1_x,set_1_y,'x-');
hold;
plot(set_2_x,set_2_y,'o-');
subplot(1,2,2);
plot(set_2_x,set_2_y,'x-');
hold;
plot(set_1_projected_x,set_1_projected_y,'o-');

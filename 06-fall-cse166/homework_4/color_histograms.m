% Author: <njoffe@ucsd.edu> Nitay Joffe
% Date: 10/24/2006
% Class: CSE 166 - Image Processing
% Homework: 1
% Problem: 3 - Color Histograms
clear;

% (b) Using the above function, compute and display the color histogram for each
%     of five different rectangular regions of your choice for Figure 6.30(a)
%     (a bowl of strawberries). Use bar to plot each histogram, and clearly mark
%     the x-axis to indicate the division between the R, G, and B intervals.
%     Annotate your plot (use gtext or just mark it by hand) to explain the
%     significance of the various peaks you observe. Along with each histogram,
%     display the corresponding region of the image used to compute it.
number_of_regions = 5;

rgb_image = imread('Fig6.30(a).jpg');

figure;
imshow(rgb_image);
region_images = {};
region_histograms = {};
region_positions = {};
for i = 1:number_of_regions
  [region_images{i},region_histograms{i},region_positions{i}] = rgb_histogram(rgb_image);
end

figure;
for i = 1:number_of_regions
  subplot(number_of_regions,2,i*2-1);
  imshow(region_images{i});
  subplot(number_of_regions,2,i*2);
  bar(region_positions{i},region_histograms{i});
end

% (c) Use this function to compare all pairs of the five histograms you computed
%     in the previous step. Output the values in a 5 by 5 matrix; because of
%     symmetry, only the upper triangular portion of this matrix is needed.
%     Choose at least three representative entries in this matrix and explain
%     their values in terms of the color distributions you compared to obtain
%     them.
chi_squared_distance = [];
for i=1:number_of_regions
  for j=1:number_of_regions
    chi_squared_distance(i,j) = chi_squared(region_histograms{i},region_histograms{j});
  end
end
chi_squared_distance
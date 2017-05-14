% Author: <njoffe@ucsd.edu> Nitay Joffe
% Date: 10/12/2006
% Class: CSE 166 - Image Processing
% Homework: 2
% Problem: 2 - Canny Edge Detection
% Section: c
clear;

% (c) Run your edge detector on Figure 10.4(a) using tau = 5 and the following
%     three values for sigma^2 : 0.5, 1, and 3. (Note: to save ink, invert E
%     before printing it.) Discuss how the choice of sigma^2 effects the
%     results.
I = double(imread('Fig10.04(a).jpg'));
threshold_tau = 5;
sigma_squared_values = [0.5 1 3];
number_of_items = numel(sigma_squared_values);

figure;
for i = 1:number_of_items
  sigma_squared = sigma_squared_values(i);
  sigma = sqrt(sigma_squared);
  [E,M,A] = canny(I,sigma,threshold_tau);

  E = ~E;

  subplot(1,number_of_items,i);
  imshow(E);
  title(['edges detected, sigma^2 = ' num2str(sigma_squared)]);
end
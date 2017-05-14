% Author: <njoffe@ucsd.edu> Nitay Joffe
% Date: 11/30/2006
% Class: CSE 166 - Image Processing
% Homework: 7
% Problem: 1 - Hough Transform
% Questions: b,c,d
clear;

% (b) Produce a simple 11 x 11 test image made up of zeros with 5 ones in it,
%     arranged like the 5 points in GW Figure 10.20(a).
test_image=zeros(11,11);
test_image(1,1)=1;
test_image(6,6)=1;
test_image(11,1)=1;
test_image(1,11)=1;
test_image(11,11)=1;

% Compute and display its HT; the result should look like GW Figure 10.20(b).
[test_image_ht,rho_max,theta_max]=hough_transform(test_image);

% Now threshold the HT to find the (\rho, \theta)-coordinates of cells with more
% than 2 votes and plot the corresponding lines in (x, y)-space on top of the
% original image.
[rho,theta]=find(test_image_ht>2);
rho=rho-rho_max-1;
theta=theta-theta_max-1;

[test_image_y_limit test_image_x_limit]=size(test_image);
test_image_line_functions=cell(numel(rho),1);
for i=1:numel(rho)
  test_image_line_functions{i}=@(x)(rho(i)-x.*cosd(theta(i)))/sind(theta(i));
end

% (c) Load in the matchstick image in GW Figure 8.02(a) and shrink it to half
%     its size using I=imresize(I,0.5,'bil','crop');.
matchsticks_image=imread('Fig8.02(a).jpg');
matchsticks_image=imresize(matchsticks_image,0.5,'bil');

% Compute and display its edges using the Sobel operator with default threshold
% settings, i.e. BW=edge(I,'sobel');.
BW=edge(matchsticks_image,'sobel');

% Now compute and display the HT of BW. As before, threshold the HT and plot the
% corresponding lines atop the original image; this time, use a threshold of 50%
% of the maximum accumulator count over the entire HT.
[matchsticks_ht,rho_max,theta_max]=hough_transform(BW);

threshold=max(max(matchsticks_ht))/2;
[rho,theta]=find(matchsticks_ht>threshold);
rho=rho-rho_max-1;
theta=theta-theta_max-1;

[matchsticks_y_limit matchsticks_x_limit]=size(BW);
matchsticks_line_functions=cell(numel(rho),1);
for i=1:numel(rho)
  matchsticks_line_functions{i}=@(x)(rho(i)-x.*cosd(theta(i)))/sind(theta(i));
end

% (d) Repeat the previous step for another image of your choice. The image can
%     be from the textbook or elsewhere, but its size must be at least 128x128
%     and it should contain several extended straight lines.
house_image=imread('Fig10.10(a).jpg');
house_image=imresize(house_image,0.2,'bil');
BW=edge(house_image,'sobel');
[house_image_ht,rho_max,theta_max]=hough_transform(BW);

threshold=max(max(house_image_ht))/2;
[rho,theta]=find(house_image_ht>threshold);
rho=rho-rho_max-1;
theta=theta-theta_max-1;

[house_y_limit house_x_limit]=size(BW);
house_line_functions=cell(numel(rho),1);
for i=1:numel(rho)
  house_line_functions{i}=@(x)(rho(i)-x.*cosd(theta(i)))/sind(theta(i));
end


figure;
subplot(1,2,1);
imshow(test_image_ht);
title('Hough transform of test image with 5 white dots');
subplot(1,2,2);
imshow(test_image);
hold on;
for i=1:numel(test_image_line_functions)
  fplot(test_image_line_functions{i},[1 test_image_x_limit 1 test_image_y_limit]);
end
title('Test image with lines found from Hough Transform in blue');

figure;
subplot(1,2,1);
imshow(matchsticks_ht);
title('Hough transform of matchsticks image');
subplot(1,2,2);
imshow(matchsticks_image);
hold on;
for i=1:numel(matchsticks_line_functions)
  fplot(matchsticks_line_functions{i},[1 matchsticks_x_limit 1 matchsticks_y_limit]);
end
title('Matchsticks image with hlines found from Hough transform in blue');

figure;
subplot(1,2,1);
imshow(house_image_ht);
title('Hough transform of house image');
subplot(1,2,2);
imshow(house_image);
hold on;
for i=1:numel(house_line_functions)
  fplot(house_line_functions{i},[1 house_x_limit 1 house_y_limit]);
end
title('House image with lines found from Hough transform in blue');

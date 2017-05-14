% Author: <njoffe@ucsd.edu> (Nitay Joffe)
% Date: 10/03/2006
% Class: CSE 166 - Image Processing
% Homework: 1
% Problem: 1 - Working with plots and images in Matlab.

% Part (a).
% Use imread to load in the image.
image_uint8 = imread('Fig1.14(c).jpg');

% Convert the image from uint8 to double.
image_double = double(image_uint8);

% Part (b).
% Display figure as it appears in the book, using proper aspect ratio.
imshow(image_double);
truesize;

% Interactively select a ractange roughly enclosing the bottom half of the
% middle bottle.
rectangle = getrect;

% Crop the image using selected rectangle
cropped_image = imcrop(image_double, rectangle);

% Use subplot to display the cropped image next to the original image.
subplot(1,2,1);
imshow(image_double);
subplot(1,2,2);
imshow(cropped_image);

% Display the cropped image using a gray colormap and colorbar, with maximum and
% minimum gray levels corresponding to the maximum and minimum brightness in the
% image.
max_brightness = max(max(cropped_image));
min_brightness = min(min(cropped_image));
colormap('gray');
colorbar;

% Put the titles 'original image' and 'cropped image' on the respective images.
subplot(1,2,1);
imshow(image_double);
title('original image');
subplot(1,2,2);
imshow(cropped_image);
title('cropped image');
colormap('gray');
colorbar;

% Select a point on the original image that corresponds roughly to the center of
% the cropped image using ginput.
center_of_cropped_image = ginput(1);

% Plot a large white '+' at the location of the point returned by ginput.
hold;
plot(center_of_cropped_image(1), center_of_cropped_image(2), 'w+');

% Add a silly caption to the image
text(100,100,'A silly caption');

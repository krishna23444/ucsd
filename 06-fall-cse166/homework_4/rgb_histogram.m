% Author: <njoffe@ucsd.edu> Nitay Joffe
% Date: 10/24/2006
% Class: CSE 166 - Image Processing
% Homework: 1
% Problem: 3 - Color Histograms
% Question: a

% (a) Make a function in Matlab that computes the RGB color histogram for a
%     user-selected rectangular region in an arbitrary color image. Use 32 bins
%     for each color channel, spaced equally between 0 and 255. Concatenate the
%     three histograms together to make a combined histogram of length
%     3x32 = 96. Hint: to get the histogram for the red channel,
%     use rh=hist(R(:),32), where R is an array containing the red slice of an
%     RGB image. Once you have computed the combined histogram, normalize it so
%     that it sums to 1.
function [rgb_image_rectangle,combined_histogram,combined_position] = rgb_histogram(rgb_image)
  number_of_bins = 32;

  % We assume imshow(rgb_image) has already been displayed by the caller.
  rgb_image_rectangle = getrect;
  xmin = rgb_image_rectangle(1);
  ymin = rgb_image_rectangle(2);
  xmax = xmin + rgb_image_rectangle(3);
  ymax = ymin + rgb_image_rectangle(4);

  rgb_image_rectangle = rgb_image(ymin:ymax,xmin:xmax,:);

  red_layer_double = double(rgb_image_rectangle(:,:,1));
  [red_histogram,red_position] = hist(red_layer_double(:), number_of_bins);

  green_layer_double = double(rgb_image_rectangle(:,:,2));
  [green_histogram,green_position] = hist(green_layer_double(:), number_of_bins);
  green_position = green_position + red_position(end);

  blue_layer_double = double(rgb_image_rectangle(:,:,3));
  [blue_histogram,blue_position] = hist(blue_layer_double(:), number_of_bins);
  blue_position = blue_position + green_position(end);

  combined_histogram = [red_histogram green_histogram blue_histogram];
  combined_histogram = combined_histogram ./ sum(combined_histogram);

  combined_position = [red_position green_position blue_position];
end

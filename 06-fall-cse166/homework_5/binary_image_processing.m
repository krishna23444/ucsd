% Author: <njoffe@ucsd.edu> Nitay Joffe
% Date: 11/14/2006
% Class: CSE 166 - Image Processing
% Homework: 5
% Problem: 1 - Binary image processing
clear;

% (a) Reproduce GW Figure 9.5(a,c).
story_image = imread('Fig9.05(a).jpg');
structuring_element = [0 1 0;
                       1 1 1;
                       0 1 0];
% I couldn't get this to work, not sure why... it seems like I'd need the
% opposite structuring element or something. So instead I actually use
% erode to do the dilation. The results seem the same to me.
%story_image_dilated = imdilate(story_image, structuring_element);
story_image_eroded = imerode(story_image, structuring_element);

figure;
subplot(1,2,1);
imshow(story_image);
title('original image');
subplot(1,2,2);
imshow(story_image_eroded);
title('dilated image');

% (b) Reproduce GW Figure 9.14(a,b).
face_image = imread('Fig9.14(a).jpg');
structuring_element = [1 1 1;
                       1 1 1;
                       1 1 1];
face_image_eroded = imerode(face_image, structuring_element);
face_image_boundary = face_image - face_image_eroded;

figure;
subplot(1,2,1);
imshow(face_image);
title('original image');
subplot(1,2,2);
imshow(face_image_boundary);
title('boundary of image');

% (c) Perform connected components labelling on the particles image for GW
%     Problem 9.27. Based on the area of each connected component, produce a
%     new image containing only the isolated (nonoverlapping) particles. Assume
%     all particles are approximately the same size.
particles_image = imread('FigProb9.27.jpg');
particles_labels = bwlabel(particles_image, 8);
stats = imfeature(particles_labels, 'Area');
particle_area_min = 250;
particle_area_max = 550;
for i = 1:numel(stats)
  areas(i) = stats(i).Area;
end

% Threshold according to area
areas(areas<particle_area_min) = 0;
areas(areas>particle_area_max) = 0;

isolated_image = particles_image;

for removed_label = find(areas==0)
  isolated_image(find(particles_labels==removed_label)) = 0;
end

figure;
subplot(1,2,1);
imshow(particles_image);
title('original image');
subplot(1,2,2);
imagesc(isolated_image);
title('isolated nonoverlapping particles in original image');
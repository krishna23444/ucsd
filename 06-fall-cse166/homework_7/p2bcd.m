% Author: <njoffe@ucsd.edu> Nitay Joffe
% Date: 11/30/2006
% Class: CSE 166 - Image Processing
% Homework: 7
% Problem: 2 - Lucas-Kanader optical flow
% Questions: b,c,d
clear;

% (b) Construct two frames of a simple motion sequence as follows. Make a 16x16
%     white square centered on a black background of size 32 x 32.
square_image=zeros(32);
square_image(9:24,9:24)=1;

% Blur it with a Gaussian filter with \sigma = 1.
gaussian_filter=fspecial('gaussian',3,1);
blurred_square=conv2(square_image,gaussian_filter,'same');

% This image represents I(x, y, t).
I=blurred_square;

% Produce the second image, representing I(x, y, t + 1), by displacing the first
% image down one pixel and to the right one pixel.
I_shifted=circshift(I,[1 1]);

% Display each frame, as well as I_t and the two components of \gradient(I).
[shifted_dx shifted_dy]=gradient(I);
shifted_dt=I_shifted-I;

% (c) Compute and display the optical flow for the above sequence using a window
%     size of 5 x 5.
window_size=3;
[I_shifted_u,I_shifted_v,I_shifted_eigenvalues_min]=optical_flow(I,I_shifted,window_size);

% Since you know the "ground truth" displacement (i.e. u = 1, v = 1), comment
% on the accuracy of your measured optical flow at various points throughout the
% image.

% Demonstrate how, by applying a threshold on the eigenvalues, you can suppress
% the flow vectors at pixels that suffer from the aperture problem.
threshold_percentage=0.1;
threshold=threshold_percentage*max(max(I_shifted_eigenvalues_min));

I_shifted_u_thresholded=I_shifted_u;
I_shifted_v_thresholded=I_shifted_v;

[rows,columns]=size(I_shifted_u);
for i=1:rows
  for j=1:columns
    if I_shifted_eigenvalues_min(i,j)<=threshold
      I_shifted_u_thresholded(i,j)=0;
      I_shifted_v_thresholded(i,j)=0;
    end
  end
end

% (d) Construct a new sequence consisting of the original first frame and a
%     second frame produced by rotating the first one by 5degrees (use imrotate
%     with the 'bil' and 'crop' options).
I_rotated=imrotate(I,5,'bil','crop');

% Now repeat step 2c using this sequence.
[I_rotated_u,I_rotated_v,I_rotated_eigenvalues_min]=optical_flow(I,I_rotated,window_size);

threshold=threshold_percentage*max(max(I_rotated_eigenvalues_min));

I_rotated_u_thresholded=I_rotated_u;
I_rotated_v_thresholded=I_rotated_v;

[rows,columns]=size(I_rotated_u);
for i=1:rows
  for j=1:columns
    if I_rotated_eigenvalues_min(i,j)<=threshold
      I_rotated_u_thresholded(i,j)=0;
      I_rotated_v_thresholded(i,j)=0;
    end
  end
end


figure;
subplot(1,5,1);
imshow(I);
title('blurred white square centered on black background');
subplot(1,5,2);
imshow(I_shifted);
title('square shifted one pixel down and right');
subplot(1,5,3);
imshow(shifted_dt);
title('derivative of brightness with respect to t (time)');
subplot(1,5,4);
imshow(shifted_dx);
title('derivative of brightness with respect to x axis');
subplot(1,5,5);
imshow(shifted_dy);
title('derivative of brightness with respect to y axis');

figure;
subplot(1,2,1);
quiver(I_shifted_u,I_shifted_v);
title(['Optical flow for shifted square sequence using window size ' num2str(window_size)]);
subplot(1,2,2);
quiver(I_shifted_u_thresholded,I_shifted_v_thresholded);
title(['Thresholded optical flow for shifted square sequence with threshold=' num2str(threshold_percentage) '% of small eigenvalue']);

figure;
subplot(1,2,1);
quiver(I_rotated_u,I_rotated_v);
title(['Optical flow for rotated square sequence using window size ' num2str(window_size)]);
subplot(1,2,2);
quiver(I_rotated_u_thresholded,I_rotated_v_thresholded);
title(['Thresholded optical flow for rotated square sequence with threshold=' num2str(threshold_percentage) '% of small eigenvalue']);

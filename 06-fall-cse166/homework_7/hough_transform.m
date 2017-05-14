% Author: <njoffe@ucsd.edu> Nitay Joffe
% Date: 11/30/2006
% Class: CSE 166 - Image Processing
% Homework: 7
% Problem: 1 - Hough Transform
% Question: a

% (a) Implement the Hough Transform (HT) using the (\rho, \theta)
%     parameterization as described in GW Section 10.2.2. Use accumulator cells
%     with a resolution of 1 degree in \theta and 1 pixel in \rho.
function [hough_space,rho_max,theta_max] = hough_transform(binary_image)
  theta_min=-90;
  theta_max=90;
  theta=theta_min:theta_max;
  cosine_theta=cosd(theta);
  sine_theta=sind(theta);

  [ones_y,ones_x]=find(binary_image);
  ones_size=numel(ones_x);
  for i=1:numel(ones_x)
    rho(i,:)=round(ones_x(i)*cosine_theta+ones_y(i)*sine_theta);
  end
  [image_rows,image_columns]=size(binary_image);
  diagonal_distance=sqrt(image_rows^2+image_columns^2);
  rho_max=round(sqrt(2)*diagonal_distance);

  rho=rho+rho_max+1;
  theta=theta+theta_max+1;

  rho_size=rho_max*2+1;
  theta_size=numel(theta);
  hough_space=zeros(rho_size,theta_size);
  for i=1:ones_size
    for j=1:theta_size
      hough_space(rho(i,j),theta(j))=hough_space(rho(i,j),theta(j))+1;
    end
  end
end

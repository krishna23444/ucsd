% Author: <njoffe@ucsd.edu> Nitay Joffe
% Date: 11/30/2006
% Class: CSE 166 - Image Processing
% Homework: 7
% Problem: 2 - Lucas-Kanader optical flow
% Question: a

% Implement the Lucas-Kanade algorithm for measuring optical flow, as
% described in class. Allow the user to specify the size of the window used
% in enforcing the smoothness constraint. Use the quiver function to
% display the optical flow vectors. In addition, have your program return the
% two eigenvalues of the windowed image second moment matrix at each pixel.
function [u,v,eigenvalues_min,eigenvalues_max]=optical_flow(first_image,second_image,window_size)
  window_delta=(window_size-1)/2;
  [dx,dy]=gradient(first_image);
  dt=second_image-first_image;

  h=ones(window_size,1);
  sum_dx_squared=conv2(h,h,dx.*dx,'same');
  sum_dy_squared=conv2(h,h,dy.*dy,'same');
  sum_dx_times_dy=conv2(h,h,dx.*dy,'same');

  [rows,columns]=size(first_image);
  for i=1:rows
    for j=1:columns
      window_rows=max(1,i-window_delta):min(rows,i+window_delta);
      window_columns=max(1,j-window_delta):min(columns,j+window_delta);

      dx_window=dx(window_rows,window_columns);
      dy_window=dy(window_rows,window_columns);
      dt_window=dt(window_rows,window_columns);

      A=[dx_window(:),dy_window(:)];
      b=dt_window(:);

      scatter_matrix_1=A'*A;
      scatter_matrix_2=[sum_dx_squared(i,j),sum_dx_times_dy(i,j);
                       sum_dx_times_dy(i,j),sum_dy_squared(i,j)];
      scatter_matrix=scatter_matrix_2;

      eigenvalues=eig(scatter_matrix);
      eigenvalues_min(i,j)=min(eigenvalues);
      eigenvalues_max(i,j)=max(eigenvalues);

      gradient_vector_1=-inv(scatter_matrix)*A'*b;
      gradient_vector_2=-A\b;
      gradient_vector=gradient_vector_2;

      u(i,j)=gradient_vector(1);
      v(i,j)=-gradient_vector(2);      
    end
  end
end

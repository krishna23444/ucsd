% Author: <njoffe@ucsd.edu> (Nitay Joffe)
% Date: 10/03/2006
% Class: CSE 166 - Image Processing
% Homework: 1
% Problem: 4 - Sampling and Aliasing

% Part (a).
x_interval = 0:16;
x_limits = [x_interval(1) x_interval(end)];
k_interval = 0:16;
N = 16;

for k = k_interval
  omega_0 = 2*pi*k/N;
  % Create cos(2*pi*k*x/N) function.
  function_handle = @(x)cos(omega_0 * x);
  % Sample values of function at x interval.
  f_n = function_handle(x_interval);

  subplot(5,4,k+1);
  % Plot continuous function.
  fplot(function_handle,x_limits);
  % Plot samples of function.  We have to specify the x interval to stem because
  % otherwise it assumes x starts at one and we need it to start at zero.
  hold;
  stem(x_interval,f_n);
  title(['k = ' int2str(k)]);
end

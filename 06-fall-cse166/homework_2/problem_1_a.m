% Author: <njoffe@ucsd.edu> Nitay Joffe
% Date: 10/12/2006
% Class: CSE 166 - Image Processing
% Homework: 2
% Problem: 1 - Binomial Approximation
clear;

% (a) Make plots comparing the binomial kernel to the continuous Gaussian
%     kernel for sigma^2 = 1.0, 2.5, 4.0, 5.5. In each plot, use stem to plot
%     the binomial kernel and fplot to plot the corresponding Gaussian on the
%     interval [-3*sigma, 3*sigma]. You will need to use hold to put both
%     functions on the same plot. Also, overlaying these correctly is a bit
%     tricky; make sure the Gaussian and binomial kernel overlap. Arrange your
%     plots on a single sheet of paper using subplot.
sigma_squared_values = [1.0, 2.5, 4.0, 5.5];

for i = 1:numel(sigma_squared_values)
  subplot(2,2,i);

  sigma_squared = sigma_squared_values(i);

  % Plot Gaussian in continuous time using fplot.
  range = [sigma_squared * -3, sigma_squared * 3];
  fplot(@(x) exp(-x^2/(2*sigma_squared)) / sqrt(2*pi*sigma_squared), range);

  hold;

  % Plot binomial approximation of Gaussian in discrete time using stem.
  % Use normalized Nth row of Pascal's triangle, where N = 4*sigma^2+1.
  pascal_row_number = 4*sigma_squared+1;
  pascal_row = diag(rot90(pascal(pascal_row_number)))';

  normalization_factor = 2.^-(pascal_row_number-1);
  normalized_pascal_row = pascal_row .* normalization_factor;

  stem_range = ceil(-pascal_row_number/2):floor(pascal_row_number/2);
  stem(stem_range, normalized_pascal_row);

  title(['sigma^2 = ' num2str(sigma_squared)]);
end

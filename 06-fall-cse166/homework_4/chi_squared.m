% Author: <njoffe@ucsd.edu> Nitay Joffe
% Date: 10/24/2006
% Class: CSE 166 - Image Processing
% Homework: 1
% Problem: 3 - Color Histograms
% Question: c

% (c) Implement a function to compute the \chi^2 distance between a pair of
%     normalized histograms h_i(k) and h_j(k), which is defined as:
%
%     \chi^2(i,j) =
%         \frac{1}{2}\sum_{k=1}^{K}\frac{[h_i(k)-h_j(k)]^2}{h_i(k)+h_j(k)}
function distance = chi_squared(normalized_histogram_1, normalized_histogram_2)
  numerator = (normalized_histogram_1 - normalized_histogram_2) .^ 2;
  denominator = normalized_histogram_1 + normalized_histogram_2;
  distance = 0.5 * sum(sum(numerator ./ denominator));
end

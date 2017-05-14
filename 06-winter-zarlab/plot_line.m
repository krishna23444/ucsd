function plot_line(slope, x, y)
  %% usage: plot_line(slope, [, x[, y]])
  %%

  if nargin < 3
    y = -10:.05:10;
  end
  if nargin < 2
    x = -10:.05:10;
  end

  if length(slope) >= 3
    z = slope(1) + slope(2) * x + slope(3) * y;
    plot3(x, y, z);
  else
    y = slope(1) + slope(2) * x;
    plot(x, y);
  end
end

% test code
plot_line([0 1])
input("done")

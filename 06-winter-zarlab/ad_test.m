function [y,col] = ad_test(x)
  %% usage: [y,col] = ad_test(x)
  %%
  %% run anderson darling test on every column of x, return the minimum value
  %% along with the column.

  for i = 1:columns(x)
    y(i) = anderson_darling_test(x(:,i), 'normal');
  end
  col = find(y == min(y))
  y = min(y);
end


% test code

function slp = cluster_slopes(x, cl)
  %% usage: slp = cluster_slopes(x [, cl])
  %%
  %% Find best-line fits for each cluser, using least squares method. Returns a
  %% matrix of line coefficients, one row per cluster. If cl is not given,
  %% assume all the data comes from one cluster, otherwise cl is a vector
  %% telling which cluster each row of data belongs to.

  if nargin < 2
    cl = ones(rows(x), 1);
  end

  % loop until there are no items in a cluster
  for i = 1:length(cl)
    % grab all the items in the current cluster
    data = x(find(cl == i));

    % quit when cluster has no items, ie there are no more clusters
    if length(data) == 0
      break;
    end

    % split up x and y data, use ols to find least squares line of best fit
    slp(i) = ols(data(:,end), data(:,1:end-1));
  end
end


% test code

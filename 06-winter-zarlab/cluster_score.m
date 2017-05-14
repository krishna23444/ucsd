function [y,col] = cluster_score(x, type)
  %% usage: y = cluster_score(x [, type])
  %%
  %% Assess and produce a score describing how likely it is that the data in x
  %% came from a cluster. type describes the type of cluster being tested
  %% against, and is either 'line' or 'image', default is 'line'.

  if nargin < 2
    type = 'line';
  end

  % handle different types of clusters
  if type == 'image'
    num_cut_dim = 3;
  else
    num_cut_dim = 1;
  end

  % use singular value decomposition to get basis for the noise around cluster
  [u,s,v] = svd(x);
  v = v(:,num_cut_dim+1:end);

  % transform data to vector space defined by basis -> multivariate normal
  noise = v' * x;

  y = ad_test(noise);
end


% test code

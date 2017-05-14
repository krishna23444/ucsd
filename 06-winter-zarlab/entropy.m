function h = entropy(x)
  %% usage: h = entropy(x)
  %%
  %% Compute discrete entropy of the columns of x.
  %%
  %%  h = -SUM(p(x) * log(p(x)))
  %%
  %% For the case where p(x) = 0, 0*log(0) is defined as 0.

  for i = 1:columns(x)
    h(i) = 0;
    for j = 1:rows(x)
      if x(j,i) > 0
        h(i) += x(j,i) * log2(x(j,i));
      end
    end
  end
  h = -h;
end

function h = normal_entropy(v)
  %% usage: h = normal_entropy(v)
  %%
  %% Compute entropy of normal distribution with covariance matrix v
  %%
  %%  h(g) = -INTEGRAL(g * log(g) * dt) = log(2 * pi * e * VAR) / 2
  %%
  %% where VAR is the variance of the gaussian g.

  h = 0.5 * (rows(v) * (1 + log(2 * pi)) + log(det(v)));
end

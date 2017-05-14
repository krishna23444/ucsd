function y = dist_to_cluster(x, slope)
  %% usage: y = dist_to_cluster(x, slope)
  %%
  %% Compute distance from point x to clusters defined by slope. Basically
  %% does a point-to-line distance of x to the lines defined by slope. The
  %% first column of slope for each row is the line's offset from the origin.

  % split slope matrix into y intercepts and directions, for each line
  y_intercepts = slope(:,1)
  lines = slope(:,2:end)

  for i = 1:rows(slope)
    % translate data so that it corresponds with the same line at the origin
    x_shifted = [x(1:end-1) x(end)-y_intercepts(i)]

    % get unit vector in direction of line
    lines(i) = lines(i) / norm(lines(i))

    % distance is magnitude of vector from point to its projection onto line
    y(i) = sqrt(sumsq(x_shifted - dot(x_shifted, lines(i))))
  end
end

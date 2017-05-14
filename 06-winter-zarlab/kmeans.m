function [ncl, nslp] = kmeans(x, cl)
  %% usage: [ncl, nslp] = kmeans(x, cl)
  %%
  %% Run kmeans on the data in x, moving the points between clusters in
  %% cl using a distance heuristic. Algorithm converges when no more
  %% improvement can be made by moving a point from one cluster to another.
  %%
  %% x is the data, cl is the initial cluster assignments. x and cl are vectors
  %% of the same length.
  %%
  %% Returns the new cluster assignments in ncl, and the new cluster line
  %% slopes in nslp (which are computed from ncl).

  cl_slopes = cluster_slopes(x, cl)

  num_clusters = max(cl)

  % tells if a move happened during current round of update
  changed = 1

  while changed
    changed = 0
    for i = 1:length(x)
      curr_cluster = cl(i)

      % calculate point's distance to all the clusters
      dist = dist_to_clusters(x(i), cl_slopes)
      min_dist_cluster = find(dist == min(dist))
      min_dist = dist(min_dist_cluster)

      % update, if some other cluster is closer to our point
      if (curr_cluster != min_dist_cluster)
        changed = 1
        % move point
        cl(i) = min_dist_cluster
        % recompute slopes of cluster that lost point and cluster and gained it
      end
    end
  end
end

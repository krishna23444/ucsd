d = 2;
n = 1000;

x = rand(n,d);
slp = cluster_slope(x);

clearplot
hold on
plot(x(:,1), x(:,2), '@')
hold off
input("done ")

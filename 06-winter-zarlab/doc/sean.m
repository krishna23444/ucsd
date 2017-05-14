global BETA PVAL KMEANSP
BETA = [0.1 1 1];
PVAL = 0.95;
KMEANSP = 0;

function y = adtest(X)
    ## usage: y = adtest(X)
    ##
    ## Perform Anderson-Darling test in every dimension on X
    for i = 1:columns(X)
        y(i) = anderson_darling_test(X(:,i), 'normal');
    end
    y = min(y);
end

function [y,sc1, sc2, sc3] = cluster_score(x)
    ## usage: y = cluster_score(x)
    ##
    ## Compute the cluster score for elements X.  For "G-means", the
    ## score is
    ##
    ##     B1 * H(X) + B2 * adtest(X) + B3 * MI(X)
    ##
    ## For k-means, it's the log-odds score of the data.

    global BETA KMEANSP
    if KMEANSP
        sc1 = sc2 = sc3 = 0;
        m = mean(x);
        v = var(x);
        y = -sum(log(normal_pdf(x(1,:),m(1),v(1))));
        for i = 2:columns(x)
            y -= sum(log(normal_pdf(x(i,:),m(i),v(i))));
        end
    else
        sc1 = BETA(1) * log2(rows(x));
        sc2 = BETA(2) * log2(adtest(x, 'normal'));
        sc3 = BETA(3) * normal_i(x);
        y = -sc1 - sc2 + sc3;
    end
end

function [ix,changed] = kmeans_seq(x, ix)
    ## usage: [ix, changed] = kmeans_seq(x, ix)
    ##
    ## Perform sequential updates between clusters IX of data X,
    ## returning the new cluster assignment IX and the number of changes
    ## made.

    sc = zeros(length(ix));
    for i = 1:length(ix)
        sc(i) = cluster_score(x(ix{i},:));
    end
    changed=0;
    ch=1;
    while ch
    ch = 0;
    sc0 = sum(sc);
    for i = 1:length(ix)
        ixi = ix{i};
        if length(ixi) <= 8, continue; end
        for j = 1:length(ix)
            if i == j, continue; end
            ii = 1;
            while ii < length(ixi)
                newsci = cluster_score(x(ixi([1:ii-1,ii+1:end]),:));
                di = newsci - sc(i);
                newscj = cluster_score(x([ix{j}, ixi(ii)],:));
                dsc = newscj - sc(j) + di;
                if dsc < 0
                    ## Update
                    changed = 1; ch=1;
                    ix{j} = [ix{j},ixi(ii)];
                    fprintf(stderr, "move %d: %d -> %d, dsc = %g\n",
                            ixi(ii), i, j, dsc);
                    sc(i) = newsci;
                    sc(j) = newscj;
                    ixi = ixi([1:ii-1,ii+1:end]);
                    ix{i} = ixi;
                    if length(ixi) <= 8, break; end
                end
                ii = ii + 1;
            end
        end
    end
    end # while changed
    fprintf(stderr, "kmeans_seq(): %g -> %g\n", sc0, sum(sc));
end

function ix = gmeans(x, ix)
    ## usage: ix = gmeans(x [, ix])
    ##
    ## Perform sequential top-down splitting clustering on points X,
    ## returning the cluster assignments.  IX, if supplied, is an
    ## initial cluster assignment.
    ##
    ## XXX: this may work for non-2-dimensional X, but hasn't been
    ## tested.

    global PVAL IX KMEANSP
    initval = nargin > 1;
    if !initval
        ix = { 1:rows(x) };
    end
    while !KMEANSP || initval || length(ix) < KMEANSP
        ## Try split
        if initval
            ## Skip splits the first time through
            initval = 0;
            i = length(ix) + 1;
            changed = 1;
        else
            i = 1;
            changed = 0;
        end
        fprintf(stderr, ".");
        while i <= length(ix)
            if length(ix{i}) < 16
                i = i + 1;
                continue;
            end
            ad = adtest(x(ix{i},:), 'normal');
            if ad < PVAL
                fprintf(stderr, "Try split %d (ad = %g)\n", i, ad);
                if columns(x) == 1
                    [xx,ixi] = sort(x(ix{i}));
                    tmp = floor(length(ixi)/2);
                    foo = sort(ix{i}(ixi(1:tmp)));
                    bar = sort(ix{i}(ixi(tmp+1:end)));
                else
                    xs = x(ix{i},:);
                    v = princomp(xs);
                    pr = xs * v(:,1);
                    foo = sort(ix{i}(find(pr < 0)));
                    bar = sort(ix{i}(find(pr >= 0)));
                end
                tmp = kmeans_seq(x, { foo, bar });
                [a1,b1,c1,d1] = cluster_score(x(tmp{1},:));
                [a2,b2,c2,d2] = cluster_score(x(tmp{2},:));
                newsc = (a1 + a2)/2;
                [oldsc,b0,c0,d0] = cluster_score(x(ix{i},:));
                fprintf(stderr, "Split %d: (%g,%g,%g) -> (%g,%g,%g) + (%g,%g,%g)\n",
                        i, b0, c0, d0, b1, c1, d1, b2, c2, d2);
                if newsc < oldsc
                    ix = { ix{1:i-1}, tmp{:}, ix{i+1:end} };
                    fprintf(stderr, "DID SPLIT %d\n", i);
                    changed += 1;
                    i = i + 2;
                else
                    fprintf(stderr, "No split %d: %g <= %g\n", i,oldsc, newsc);
                    i = i + 1;
                end
            else
                fprintf(stderr, "Not trying to split: ad = %g\n", ad);
                i = i + 1;
            end
        end
        if !changed, break; end
        fprintf(stderr, "CHANGED %d\n", changed);
        ## Okay, try global seq updates
        ix = kmeans_seq(x, ix);
        ## A bit of debugging verbosity:
        IX = ix
        for i = 1:length(ix)
            [y,sc1,sc2,sc3] = cluster_score(x(ix{i},:));
            fprintf(stderr, "[id=%d,n=%d,sc=%g (%g, %g, %g)]\n",
                    i, length(ix{i}), y, sc1, sc2, sc3);
            fdisp(stderr, mean(x(ix{i},:)));
            fdisp(stderr, std(x(ix{i},:)));
        end
    end
end

function multihist(n, varargin)
    ## usage: multihist(n, varargin)
    ##
    ## Plot multiple N-bin histograms on a single plot, one for each
    ## argument.

    things = {};
    for i = 1:length(varargin)
        [nn,xx] = hist(varargin{i}, n, 1);
        [xx,yy] = bar(xx,nn);
        things = { things{:}, xx, yy };
    end
    plot(things{:});
end

function X = multinormal_rnd(m,v,n)
    ## usage: X = multinormal_rnd(m,v,n)
    ##
    ## Generate n points sampled from the multivariate normal
    ## distribution with mean M and diagonal covariance V.

    X = [];
    for i = 1:length(m)
        X = [X, normal_rnd(m(i), v(i), n, 1)];
    end
end

function y = multinormal_pdf(X, m, Cov)
    ## usage: y = multinormal_pdf(X, m, Cov)
    ##
    ## Compute the multivariate normal PDF at each value X(i,:) in X.
    ## Default values are M = 0, COV = identity.

    [n,dim] = size(X);
    if nargin < 2, m = zeros(1,dim); end
    if nargin < 3, Cov = eye(dim); end
    Icov = inv(Cov);
    X -= repmat(m, n, 1);
    y = zeros(n, 1);
    denom = sqrt((2*pi)**dim * det(Cov));
    for i = 1:n
        y(i) = exp(-0.5 * X(i,:) * Icov * X(i,:)') / denom;
    end
end

function y = multinormal_diag_pdf(X, m, v)
    ## usage: y = multinormal_diag_pdf(X, m, v)
    ##
    ## Compute the multivariate normal PDF at each value X(i,:) assuming
    ## that each column X(:,j) is drawn from an independent normal
    ## distribution with mean m(j) and variance v(j).

    [n,dim] = size(X);
    if nargin < 2, m = zeros(1,dim); end
    if nargin < 3, v = ones(1,dim); end
    y = normal_pdf(X(:,1), m(1),v(1));
    for i = 2:dim
        y = y .* normal_pdf(X(:,i), m(i),v(i));
    end
end

function y = normal_h(Cov)
    ## usage: y = normal_h(Cov)
    ##
    ## Compute the entropy of a normal distribution with covariane
    ## matrix COV.

    n = rows(Cov);
    y = 0.5 * (n * (1 + log(2*pi)) + log(det(Cov)));
end

function y = normal_i(x)
    ## usage: y = normal_i(x)
    ##
    ## Compute the mutual information between dimensions of X, taken to
    ## be drawn from a multivariate normal distribution.

    c = cov(x);
    y = -normal_h(c);
    for i = 1:rows(c)
        y += normal_h(c(i,i));
    end
end

function Y = fix_logpr(X)
    ## usage: Y = fix_logpr(X)
    ##
    ## Given a matrix of log p(D|H), compute p(H|D), i.e.
    ##
    ##     p(h_i|D) = p(D|h_i) / \sum_j p(D|h_j)
    ##
    ## expanding the sum, where a > b, via
    ##
    ##     e^a + e^b = e^a (1 + e^(b-a))
    ##
    ## to handle small values.

    X = sort(X, 2, 'descend');
    ## XXX: is it enough to do this only once?
    psum = X(:,1) \
        + log(1 + sum(exp(X(:,2:end) - repmat(X(:,1), 1, columns(X)-1)),2));
    Y = exp(X - repmat(psum, 1, columns(X)));
end

#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <algorithm>
#include <bitset>
#include <fstream>
#include <iostream>
#include <numeric>
#include <set>
#include <sstream>
#include <stack>
#include <string>
#include <vector>
#include <map>
#include <queue>
using namespace std;

struct edge {
    int fid, tid, cost;
    bool flag;
};

bool operator<(const edge& n1, const edge& n2) {
    return (n2.cost <= n1.cost);
}

class node { public:
    int id;
    int num;
    vector<edge> e;
};

vector<node> g;
vector<vector<int> > lc;

void dji(int n) {
    if (n == 2)
        return;

    vector<int> found(g.size(), false);

    priority_queue<edge> pq;

    for (unsigned int i = 0; i < g[n].e.size(); ++i)
        pq.push(g[n].e[i]);

    found[n] = true;

    edge e;
    while (!pq.empty()) {
        e = pq.top();
        pq.pop();

        if ((lc[n][e.fid] + e.cost) < lc[n][e.tid]) {
            lc[n][e.tid] = lc[n][e.fid] + e.cost;
            if (found[e.tid] == false) {
                for (unsigned int i = 0; i < g[e.tid].e.size(); ++i)
                    pq.push(g[e.tid].e[i]);
                found[e.tid] = true;
            }
        }
    }
}

void bfs() {
    queue<edge> q;
    for (unsigned int i = 0; i < g[1].e.size(); ++i)
        q.push(g[1].e[i]);

    edge e;
    while (!q.empty()) {
        e = q.front();
        q.pop();
        if (lc[e.tid][2] < lc[e.fid][2]) {
            g[e.tid].num += g[e.fid].num + 1;
            for (unsigned int i = 0; i < g[e.tid].e.size(); ++i) {
                q.push(g[e.tid].e[i]);
            }
        }
    }

    cerr << g[2].num << "\n";
}

int main(int argc, char *argv[]) {
    ifstream fin("forest.in");

    int n, m;
    fin >> n;
    while (n > 0) {
        g.resize(n + 1);
        for (unsigned int i = 0; i < g.size(); ++i) {
            g[i].id = i;
            g[i].num = 0;
            g[i].e.clear();
        }

        lc.resize(n + 1);
        for (unsigned int i = 0; i < lc.size(); ++i) {
            lc[i].resize(n + 1);
            for (unsigned int j = 0; j < lc[i].size(); ++j) {
                lc[i][j] = INT_MAX;
            }
            lc[i][i] = 0;
        }

        fin >> m;

        int a, b, c;
        edge ee;
        for (int i = 0; i < m; ++i) {
            fin >> a >> b >> c;
            ee.fid = a; ee.tid = b; ee.cost = c;
            g[a].e.push_back(ee);
            ee.fid = b; ee.tid = a; ee.cost = c;
            g[b].e.push_back(ee);
        }
        
        for (int i = 1; i < n + 1; ++i)
            dji(i);

        bfs();

        fin >> n;
    }
}


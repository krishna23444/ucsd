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

int n, k;
vector<double> w, l;

void pp() {
    for (int i = 1; i < n + 1; ++i) {
//        cerr << i << ": " << w[i] << ", " << l[i] << "\n";
        if (w[i] + l[i] == 0)
            printf("-\n");
        else
            printf("%.3f\n", w[i]/(w[i] + l[i]));
    }
}

int main(int argc, char *argv[]) {
    ifstream fin("rps.in");

    int run = 0;
    int p1, p2;
    string m1, m2;
    fin >> n;
    while (n > 0) {
        fin >> k;
        w.clear(); l.clear();
        w.resize(n + 1); l.resize(n + 1);
        for (int i = 0; i < (k * n * (n - 1)) / 2; ++i) {
            fin >> p1 >> m1 >> p2 >> m2;
            if (((m1 == "paper") && (m2 == "rock")) ||
                ((m1 == "scissors") && (m2 == "paper")) ||
                ((m1 == "rock") && (m2 == "scissors"))) {
                w[p1]++;
                l[p2]++;
            }
            else if (((m2 == "paper") && (m1 == "rock")) ||
                    ((m2 == "scissors") && (m1 == "paper")) ||
                    ((m2 == "rock") && (m1 == "scissors"))) {
                w[p2]++;
                l[p1]++;
            }
        }

        if (run > 0)
            printf("\n");
        pp();
        fin >> n;
        ++run;
    }
}


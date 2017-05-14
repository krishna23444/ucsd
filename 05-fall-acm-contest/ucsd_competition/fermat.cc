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
#define FOR(i,a,b)  for(__typeof(b) i = (a); i <= (b); ++i)
#define REP(i,n)    for(__typeof(n) i = 0; i < (n); ++i)
using namespace std;
typedef vector<int> VI; typedef vector<vector<int> > VVI;
typedef vector<double> VD; typedef vector<vector<double> > VVD;
typedef vector<string> VS; typedef vector<vector<string> > VVS;

int main(int argc, char *argv[]) {
    ifstream fin("fermat.in");
    int n, s;

    fin >> s;
    for (int i = 0; i < s; ++i) {
        n = -1;
        fin >> n;
        f ((n == 1) || (n == 2))
            cout << "yes\n";
        else if (n != -1)
            cout << "no\n";
    }
}


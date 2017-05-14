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
    ifstream fin("compress.in");

    int i = 0;
    int n = 0, dig;
    while (!fin.eof()) {
        dig = -1;
        fin >> dig;
        if (dig != -1) {
            ++i;
            n = n ^ dig;
        }
    }
    cout << n << "\n" << i << "\n";
}


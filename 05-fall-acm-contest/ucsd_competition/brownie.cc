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
    ifstream fin("brownie.in");
    VI px, py;

    int num = 0;
    int midx = 0, midy = 0;
    int s, o;
    fin >> num;
    while (num > 0) {
        s = 0; o = 0;
        midx = 0; midy = 0;
        px.resize(num);
        py.resize(num);
        for (int i = 0; i < num; ++i)
            fin >> px[i] >> py[i];

        midx = px[num/2];
        midy = py[num/2];
        for (int i = 0; i < num; ++i) {
            if (((px[i] > midx) && (py[i] > midy)) ||
               ((px[i] < midx) && (py[i] < midy)))
                ++s;
            else if (((px[i] < midx) && (py[i] > midy)) ||
                    ((px[i] > midx) && (py[i]) < midy))
                ++o;
        }

        cout << s << " " << o << "\n";
        num = -1;
        fin >> num;
    }
}


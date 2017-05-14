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

vector<int> num(31, 0);

int d(int n) {
    if (num[n] > 0)
        return num[n];

    for (int i = 2; i < n; i += 2) {
        num[i] += d(i) * d(n - i);
    }

    return num[n];
}

int main(int argc, char *argv[]) {
    ifstream fin("domino.in");

    num[0] = 1;
    num[2] = 3;
    num[8] = 153;

    for (int i = 0; i <= 30; i += 2) {
        d(i);
    }

    for (unsigned int i = 0; i < num.size(); ++i)
        cerr << i << ": " << num[i] << "\n";

    int n;
    fin >> n;
    while (n >= 0) {
        fin >> n;
    }
}


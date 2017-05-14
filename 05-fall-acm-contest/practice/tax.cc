#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <sstream>
#include <stack>
#include <string>
#include <vector>
using namespace std;

typedef struct person {
    string ssn;
    int file;
    double income;
    double liabl;
    double amt;
};

typedef struct week {
    int numchk;
    double amt;
};

vector<person> ppl;
map<int, string> m;
week weeks[11];

int main() {
    ifstream fin("tax.in");
    person p;
    double l1, l2, l3;

    for (int i = 0; i < 11; ++i) {
        weeks[i].numchk = 0;
        weeks[i].amt = 0.0;
    }
    m[1] = "July 23";
    m[2] = "July 30";
    m[3] = "August 6";
    m[4] = "August 13";
    m[5] = "August 20";
    m[6] = "August 27";
    m[7] = "September 3";
    m[8] = "September 10";
    m[9] = "September 17";
    m[10] = "September 24";

    fin >> p.ssn;
    while (!p.ssn.empty()) {
        fin >> p.file;
        fin >> p.income;
        fin >> p.liabl;

        if (p.file == 1 || p.file == 3)
            l1 = 300.0;
        else if (p.file == 4)
            l1 = 500.0;
        else
            l1 = 600.0;
        l2 = .05 * p.income;
        l3 = p.liabl;

        if ((l1 <= l2) && (l1 <= l3))
            p.amt = l1;
        else if ((l2 <= l3) && (l2 <= l1))
            p.amt = l2;
        else
            p.amt = l3;

        ppl.push_back(p);

        int wk, wknum;
        stringstream ss(p.ssn.substr(9, 2));
        ss >> wk;
        if (wk < 10)
            wknum = 1;
        else if ((wk >= 10) && (wk < 20))
            wknum = 2;
        else if ((wk >= 20) && (wk < 30))
            wknum = 3;
        else if ((wk >= 30) && (wk < 40))
            wknum = 4;
        else if ((wk >= 40) && (wk < 50))
            wknum = 5;
        else if ((wk >= 50) && (wk < 60))
            wknum = 6;
        else if ((wk >= 60) && (wk < 70))
            wknum = 7;
        else if ((wk >= 70) && (wk < 80))
            wknum = 8;
        else if ((wk >= 80) && (wk < 90))
            wknum = 9;
        else
            wknum = 10;

        weeks[wknum].numchk++;
        weeks[wknum].amt += p.amt;

        p.ssn.clear();
        fin >> p.ssn;
    }

    for (uint i = 0; i < ppl.size(); ++i)
        printf("%s  $%.2f\n", ppl[i].ssn.c_str(), ppl[i].amt);

    for (int i = 1; i < 11; ++i) {
        if (weeks[i].numchk > 0)
            printf("%d  $%.2f %s\n", weeks[i].numchk,
                    weeks[i].amt, m[i].c_str());
    }

    return 0;
}

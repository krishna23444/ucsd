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


vector<string> cour_taken;
vector<map<string, bool> > categ;
vector<int> cr, ct;
int k, m, c;


int main(int argc, char *argv[]) {
    ifstream fin("classes.in");

    string s;
    fin >> k;
    while (k > 0) {
        fin >> m;

        cour_taken.clear();
        cr.resize(m);
        ct.resize(m);
        categ.resize(m);
        for (int i = 0; i < m; ++i) {
            cr[i] = 0;
            ct[i] = 0;
            categ[i].clear();
        }

        for (int i = 0; i < k; ++i) {
            fin >> s;
            cour_taken.push_back(s);
        }

        for (int i = 0; i < m; ++i) {
            fin >> c;
            fin >> cr[i];
            for (int j = 0; j < c; ++j) {
                fin >> s;
                categ[i][s] = true;
            }
        }

        /*
        for (unsigned int i = 0; i < categ.size(); ++i) {
            cerr << "categ " << i << ": ";
            for (map<string, bool>::iterator it = categ[i].begin();
                    it != categ[i].end(); ++it) {
                cerr << it->first << " ";
            }
            cerr << "\n";
        }

        cerr << "cour taken: ";
        for (unsigned int i = 0; i < cour_taken.size(); ++i)
            cerr << cour_taken[i] << " ";
        cerr << "\n";
        */

        for (int i = 0; i < k; ++i) {
            for (int j = 0; j < m; ++j) {
                if (categ[j].find(cour_taken[i]) != categ[j].end()) {
                    ct[j]++;
                }
            }
        }

        bool pass = true;
        for (int i = 0; i < m; ++i) {
            if (ct[i] < cr[i]) {
                pass = false;
                break;
            }
        }

        if (pass)
            cout << "yes\n";
        else
            cout << "no\n";

        fin >> k;
    }
}


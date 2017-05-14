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

struct car {
    int id;
    int arriv_t;
};


int n, t, m;
vector<int> times;
queue<car> l, r;
int timet = 0;
bool car_waiting;


void update() {
    car_waiting = (!l.empty() && l.front().arriv_t <= timet) ||
                  (!r.empty() && r.front().arriv_t <= timet);
}

void run() {
    bool left_bank = true;
    vector<car> load;

    while (l.empty() == false || r.empty() == false) {
        load.clear();
        update();

        while (!car_waiting) {
            timet += 1;
            update();
        }

        if (left_bank) {
            while ((int)load.size() < n && !l.empty() &&
                    l.front().arriv_t <= timet) {
                load.push_back(l.front());
                l.pop();
            }
            left_bank = false;
        }
        else {
            while ((int)load.size() < n && !r.empty() &&
                    r.front().arriv_t <= timet) {
                load.push_back(r.front());
                r.pop();
            }
            left_bank = true;
        }
        timet += t;
        for (unsigned int i = 0; i < load.size(); ++i) {
            times[load[i].id] = timet;
        }
    }

    for (unsigned int i = 0; i < times.size(); ++i) {
        cout << times[i] << endl;
    }
}

int main(int argc, char *argv[]) {
    ifstream fin("ferry.in");

    car c;
    string s;
    int time;

    int num;
    fin >> num;

    for (int i = 0; i < num; ++i) {
        timet = 0;

        fin >> n >> t >> m;
        times.resize(m);

        for (int j = 0 ; j < m; ++j)
            times[j] = 0;

        for (int j = 0; j < m; ++j) {
            fin >> time >> s;
            c.id = j;
            c.arriv_t = time;
            if (s == "left")
                l.push(c);
            else
                r.push(c);
        }

        if (i > 0)
            cout << endl;

        run();
    }
    return 0;
}


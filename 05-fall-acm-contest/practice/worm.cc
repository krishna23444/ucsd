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

string s;
string tok[7];

map<int, string> months;

typedef struct date {
    int mm;
    int dd;
    int hr;
    map<string, bool> ips;
    int count;
};

bool operator< (const date& d1, const date& d2) {
    if (d1.mm < d2.mm) return true;
    else if (d1.mm > d2.mm) return false;

    if (d1.dd < d2.dd) return true;
    else if (d1.dd > d2.dd) return false;

    if (d1.hr < d2.hr) return true;
    else if (d1.hr > d2.hr) return false;

    else return false;
}

map<string, date> ww;
map<string, date>::iterator it;

void gt() {
    uint a;
    for (int i = 0; i < 7; ++i) {
        if (i == 6)
            tok[i] = s;
        else {
            a = s.find_first_of(" ");
            tok[i] = s.substr(0, a);
            while (a < s.size() && s[a] == ' ')
                ++a;
            s = s.substr(a);
        }
    }

    string datetime = tok[0].substr(0, 2) + tok[0].substr(2, 2) +
                      tok[0].substr(5, 2);

    if ((it = ww.find(datetime)) != ww.end()) {
        if (it->second.ips.find(tok[1]) == it->second.ips.end()) {
            it->second.ips[tok[1]] = true;
            it->second.count++;
        }
    }
    else {
        stringstream ss;
        date ddt;

        ddt.count = 0;
        ss.str(datetime.substr(0, 2));
        ss >> ddt.mm;
        ss.clear();

        ss.str(datetime.substr(2, 2));
        ss >> ddt.dd;
        ss.clear();

        ss.str(datetime.substr(4, 2));
        ss >> ddt.hr;

        ddt.ips[tok[1]] = true;
        ddt.count = 1;

        ww[datetime] = ddt;
    }
}

int main() {
    ifstream fin("worm.in");

    months[1] = "jan";
    months[2] = "feb";
    months[3] = "mar";
    months[4] = "apr";
    months[5] = "may";
    months[6] = "jun";
    months[7] = "jul";
    months[8] = "aug";
    months[9] = "sep";
    months[10] = "oct";
    months[11] = "nov";
    months[12] = "dec";

    int i = 0;

    while (!fin.eof()) {
        ww.clear();

        getline(fin, s);
        while (!s.empty()) {
            gt();
            getline(fin, s);
        }

        if (i > 0)
            cout << endl;

        for (it = ww.begin(); it != ww.end(); ++it) {
            if (it->second.dd < 10)
                cout << "0";
            cout << it->second.dd << "-";

            cout << months[it->second.mm] << " ";

            if (it->second.hr < 10)
                cout << "0";

            cout << it->second.hr << " ";
            
            if (it->second.count > 500)
                cout << ">500";
            else
                cout << it->second.count;
            cout << "\n";
        }

        ++i;
    }

    return 0;
}

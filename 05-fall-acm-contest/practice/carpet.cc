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

typedef struct pt {
    double x, y;
};

vector<pt> pts;

int main() {
    ifstream fin("carpet.in");

    double d, ang = 0, ang2 = M_PI_2;
    pt p;

    p.x = 0; p.y = 0;
    pts.push_back(p);

    while (ang2 != 0) {
        fin >> d;
        
        p.x = d * sin(ang) + pts.back().x;
        p.y = d * cos(ang) + pts.back().y;
        pts.push_back(p);

        fin >> ang2;
        ang2 *= -(M_PI/180.0);
        ang += ang2;
    }

    double area = 0;
    for (uint i = 1; i < pts.size() - 1; ++i)
        area += ((pts[i].x-pts[0].x) * (pts[i+1].y - pts[0].y)) -
                ((pts[i+1].x-pts[0].x) * (pts[i].y - pts[0].y));

    area = fabs(area/2)/(144.0);

    cout << area << "\n";

    return 0;
}

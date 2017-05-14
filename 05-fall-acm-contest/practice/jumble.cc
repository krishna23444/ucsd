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

map<string, bool> allow;

bool real, poor, good;
string word, scram, res;

void setup() {
    allow.clear();

    string arr[] = { "AI", "AY", "EA", "EE", "EO", "IO", "OA", "OO", "OY",
                     "YA", "YO", "YU", "BL", "BR", "CH", "CK", "CL", "CR", "DR",
                     "FL", "FR", "GH", "GL", "GR", "KL", "KR", "KW", "PF",
                     "PL", "PR", "SC", "SCH", "SCR", "SH", "SHR", "SK", "SL",
                     "SM", "SN", "SP", "SQ", "ST", "SW", "TH", "THR", "TR",
                     "TW", "WH", "WR" };

    for (uint i = 0; i < sizeof(arr)/sizeof(arr[0]); ++i)
        allow[arr[i]] = true;
}

bool isvowel(char ch) {
    return (ch == 'A' || ch == 'E' || ch == 'I' || ch == 'O' ||
            ch == 'U' || ch == 'Y');
}

void isreal() {
    string s2, s3;

    real = true;
    for (uint i = 0; i < scram.size() - 1; ++i) {
        // 4 consts
        if (i < scram.size() - 3) {
            if (!isvowel(scram[i]) && !isvowel(scram[i+1]) &&
                    !isvowel(scram[i+2]) && !isvowel(scram[i+3])) {
                real = false;
                return;
            }
        }

        if (i < scram.size() - 2) {
            // 3 vows
            if (isvowel(scram[i]) && isvowel(scram[i+1]) &&
                    isvowel(scram[i+2])) {
                real = false;
                return;
            }
            
            // 3 consts not in table
            if (!isvowel(scram[i]) && !isvowel(scram[i+1]) &&
                    !isvowel(scram[i+2])) {
                if (allow.find(scram.substr(i, 3)) == allow.end()) {
                    real = false;
                    return;
                }
                else {
                    ++i;
                    continue;
                }
            }
        }

        // 2 stuff
        if ((isvowel(scram[i]) && !isvowel(scram[i+1])) ||
            (!isvowel(scram[i]) && isvowel(scram[i+1])) ||
    (!isvowel(scram[i]) && !isvowel(scram[i+1]) && scram[i] == scram[i+1]) ||
            (allow.find(scram.substr(i, 2)) != allow.end())) 
                continue;
        else {
            real = false;
            return;
        }
    }
}

void isgood() {
    good = true;

    for (uint i = 0; i < word.size(); ++i) {
        if (word[i] == scram[i]) {
            good = false;
            return;
        }
    }
}

void ispoor() {
    poor = false;

    if (word[0] == scram[0]) {
        poor = true;
        return;
    }
    
    for (uint i = 1; i < word.size() - 1; ++i) {
        if (word[i] == scram[i] && word[i+1] == scram[i+1]) {
            poor = true;
            return;
        }
    }
}

void go() {
    if (word == scram) {
        res = "not";
        return;
    }

    isreal();
    isgood();
    ispoor();

    if (real && good) {
        res = "good";
        return;
    }

    if (!real && poor) {
        res = "poor";
        return;
    }

    else
        res = "fair";
}

void print() {
    cout << scram;
    if (res == "not")
        cout << " is not a scramble of ";
    else if (res == "poor")
        cout << " is a poor scramble of ";
    else if (res == "fair")
        cout << " is a fair scramble of ";
    else
        cout << " is a good scramble of ";
    cout << word << endl;
}

int main() {
    ifstream fin("jumble.in");

    setup();

    fin >> word;
    while (!word.empty()) {
        fin >> scram;

        go();

        print();

        word.clear();
        scram.clear();
        res.clear();
        fin >> word;
    }

    return 0;
}

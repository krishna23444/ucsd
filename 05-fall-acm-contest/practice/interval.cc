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

template<class T> ostream& operator<<(ostream& o, vector<T> v) {
  o << "[";
  for (uint i = 0; i < v.size(); ++i) {
    o << v[i];
    if (i < v.size() - 1)
      o << ", ";
  }
  return o << "]";
}

typedef struct rng {
  double a, b;

  void neg() {
    double m = a;
    a = -b;
    b = -m;
  }

  void add(rng& r) {
    double i = min(a, b) + min(r.a, r.b);
    double j = max(a, b) + max(r.a, r.b);
    a = i;
    b = j;
  }

  void sub(rng& r) {
    double i = min(a, b) + max(r.a, r.b);
    double j = max(a, b) + min(r.a, r.b);
    a = i;
    b = j;
  }

  void mult(rng& r) {
    double dd[] = { a * r.a, a * r.b, b * r.a, b * r.b };
    sort(dd, dd + sizeof(dd)/sizeof(dd[0]));
    a = dd[0];
    b = dd[3];
  }

  void div(rng& r) {
    double dd[] = { a / r.a, a / r.b, b / r.a, b / r.b };
    sort(dd, dd + sizeof(dd)/sizeof(dd[0]));
    a = dd[0];
    b = dd[3];
  }
};

typedef enum { NONE, ADD, SUB, MULT, DIV } opcode;

typedef struct node {
  struct node *l, *r;
  opcode opnum;
  string stmt;
  rng ivl;

  node(opcode& o) : l(NULL), r(NULL), opnum(o), stmt(""), ivl() { }
  node(string& s) : l(NULL), r(NULL), opnum(NONE), stmt(s), ivl() { }

  ostream& operator<<(ostream& o) {
    string spc("                                                            ");
    if (l != NULL) {
      o << spc.substr(0, stmt.size()) << l << "\n";
    }
    o << stmt << "\n";
    if (r != NULL) {
      o << spc.substr(0, stmt.size()) << r << "\n";
    }
    return o;
  }
};


char chs[] = { '(', ')', '*', '/', '+', '-' };

node *root;
vector<node *> tokens;
map<char, vector<int> > ops;
string line;


void parsespc() {
  uint j = 0;
  for (uint i = 0; i < line.size(); ++i) {
    if (line[i] == ' ') {
      j = i;
      while (line[j] == ' ')
        ++j;
      line = line.substr(0, i) + line.substr(j);
    }
  }
}

void parseops() {
  for (uint i = 0; i < line.size(); ++i) {
    switch (line[i]) {
      case '[':
        while (line[i] != ']')
          ++i;
        break;
      case '(':
      case ')':
      case '*':
      case '/':
      case '+':
      case '-':
        ops[line[i]].push_back(i);
        break;
    }
  }
}

bool IsMathOp(char ch) {
  return (ch == '*' || ch == '/' || ch == '+' || ch == '-');
}

void BuildTreeFromTokens(vector<int>& tokmap) {
  string s;
  int toknum = 0;

  for (uint i = 0; i < tokmap.size(); ++i) {
    uint j = i;
    while (j < tokmap.size() && tokmap[j] == toknum)
      ++j;
    s = line.substr(i, j - i);
    i = j;
  }
}

void BuildTreeParens() {
  vector<int> toks(line.size(), 0);

  string tok1, op1, tok2, op2, tok3;

  for (int i = ops['('].size() - 1; i >= 0; --i) {
    for (uint j = 0; j < ops[')'].size(); ++j) {
      if (ops[')'][j] < ops['('][i])
        continue;

      if (ops['('][i] - 1 >= 0) {
        if (IsMathOp(line[ops['('][i] - 1])) {
          tok1 = line.substr(0, ops['('][i] - 1);
          op1 = line[ops['('][i] - 1];
        }
        else
          tok1 = line.substr(0, ops['('][i]);
      }
      tok2 = line.substr(ops['('][i] + 1, ops[')'][j] - ops['('][i] - 1);
      if (ops[')'][j] + 1 <= (int)line.size()) {
        if (IsMathOp(line[ops[')'][i] + 1])) {
          op2 = line[ops[')'][i] + 1];
          tok3 = line.substr(ops[')'][j] + 2);
        }
        else
          tok3 = line.substr(ops[')'][j] + 1);
      }

      ops['('].erase(ops['('].begin() + i);
      ops[')'].erase(ops[')'].begin() + j);
      cerr << "_" << tok1 << "_ _" << op1 << "_ _" << tok2 << "_ _" << op2 <<
            "_ _" << tok3 << "_\n";
      break;
    }
  }

  BuildTreeFromTokens(toks);
}

void parse() {
  parsespc();

  parseops();

  cerr << "_" << line << "_\n";

  BuildTreeParens();
}

int main() {
  ifstream fin("interval.in");

  vector<int> v(0);
  ops.clear();
  for (uint i = 0; i < sizeof(chs)/sizeof(chs[0]); ++i)
    ops[chs[i]] = v;

  getline(fin, line);
  while (!line.empty()) {
    parse();

    line.clear();
    for (uint i = 0; i < sizeof(chs)/sizeof(chs[0]); ++i)
      ops[chs[i]].clear();
    getline(fin, line);
  }

  return 0;
}

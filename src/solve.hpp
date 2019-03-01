#ifndef solve_hpp
#define solve_hpp

#include <bits/stdc++.h>
using namespace std;

vector<int> genAssignment(int var);
vector< vector <int> > combGen(int N, int K);
vector< vector <int> > genClausesSatFlip(vector<int> assignment,vector< vector <int> > formula,int k,int var);
vector< vector <int> > genClausesSatFlipCounter(vector<int> assignment,vector< vector <int> > formula,int k,int* var);

int valueVar(int var, vector<int> assignment);
int getIndex(int i, int j, int k);

#endif

#include "solve.hpp"

using namespace std;


vector<int> genAssignment(int var){
  srand (time(NULL));
  unsigned int i;
  vector<int> result;
  int value;

  result.push_back(0);

  for(i = 1;i<= var;i++){
    value = rand() % 2;
    result.push_back(value);
  }

  return result;

}


vector< vector <int> > combGen(int N, int K)
{
    std::string bitmask(K, 1); // K leading 1's
    bitmask.resize(N, 0); // N-K trailing 0's
    vector< vector <int> > result;
    vector<int> vecAux;
    // print integers and permute bitmask

    do {
        vecAux.clear();
        for (int i = 0; i < N; ++i) // [0..N-1] integers
        {
            if (bitmask[i]){
              vecAux.push_back(i+1);
            }
        }
        result.push_back(vecAux);

    } while (std::prev_permutation(bitmask.begin(), bitmask.end()));
    return result;
}


vector< vector <int> > genClausesSatFlip(vector<int> assignment,vector< vector <int> > formula,int k,int var){
  //generate every combination of k+1 variables
  vector< vector<int> > combinations;
  vector<int> comb;
  vector<int> clause;
  unsigned int i,j;

  combinations = combGen(var,k+1);
  //for every possible numVariablesar choose k+1 do
  for(i = 0;i<combinations.size();i++){
    //compose the clause to be added
    comb = combinations[i];
    clause.clear();
    for(j=0;j<comb.size();j++){
        if(assignment[comb[j]] == 1){
          clause.push_back(comb[j]);
        }else{
          clause.push_back(comb[j] * -1);
        }
    }
    //add the clause to the formula
    formula.push_back(clause);

  }


  return formula;


}

int getIndex(int i, int j, int k){
  //i vai de 1 a n
  //j vai de 1 a k

  return k * (i - 1) + (j - 1);

}

int valueVar(int var, vector<int> assignment){
  if(assignment[var] == 0){
    return (-1 * var);
  }else{
    return var;
  }

}

vector< vector <int> > genClausesSatFlipCounter(vector<int> assignment,vector< vector <int> > formula,int k,int* var){
  vector<int> auxClause;
  unsigned int i,j;
  int oldClausesNum;
  int newClausesNum;
  //gen clauses
  //clause 1
  oldClausesNum = formula.size();

  auxClause.push_back(valueVar(1,assignment));
  auxClause.push_back(*var + 1 + getIndex(1,1,k));

  formula.push_back(auxClause);

  //clase 2
  for(j = 2; j<= k;j++){
    auxClause.clear();
    auxClause.push_back(-1 * (*var + 1 + getIndex(1,j,k)));
    formula.push_back(auxClause);
  }

  //clause 3

  for(i = 2;i < *var;i++){
    auxClause.clear();
    auxClause.push_back(valueVar(i,assignment));
    auxClause.push_back(*var + 1 + getIndex(i,1,k));
    formula.push_back(auxClause);
    auxClause.clear();
    auxClause.push_back(-1 * (*var + 1 + getIndex(i - 1,1,k)));
    auxClause.push_back(*var + 1 + getIndex(i,1,k));
    formula.push_back(auxClause);
    for(j = 2;j<=k;j++){
      auxClause.clear();
      auxClause.push_back(valueVar(i,assignment));
      auxClause.push_back(-1 * (*var + 1 + getIndex(i - 1,j-1,k)));
      auxClause.push_back(*var + 1 + getIndex(i,j,k));
      formula.push_back(auxClause);
      auxClause.clear();
      auxClause.push_back(-1 * (*var + 1 + getIndex(i - 1,j,k)));
      auxClause.push_back(*var + 1 + getIndex(i,j,k));
      formula.push_back(auxClause);
    }
    auxClause.clear();
    auxClause.push_back(valueVar(i,assignment));
    auxClause.push_back(-1 * (*var + 1 + getIndex(i - 1,k,k)));
    formula.push_back(auxClause);
  }
  auxClause.clear();
  auxClause.push_back(valueVar(*var,assignment));
  auxClause.push_back(-1 * (*var + 1 + getIndex(*var - 1,k,k)));
  formula.push_back(auxClause);

  newClausesNum = formula.size();

  cout << "clauses antes: " << oldClausesNum << " clauses depois: " << newClausesNum << endl;
  cout << "variaveis antes: " << *var << " variaveis depois: " << (*var + (*var * k) - k) << endl;
  *var = *var + (*var * k) - k;

  return formula;
}

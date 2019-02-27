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

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

  //cout << "clauses antes: " << oldClausesNum << " clauses depois: " << newClausesNum << endl;
  //cout << "variaveis antes: " << *var << " variaveis depois: " << (*var + (*var * k) - k) << endl;
  *var = *var + (*var * k) - k;

  return formula;
}





int getNumClausesTrue(vector< vector <int> > formula, vector<int> assignment){
  int result = 0;
  unsigned int i,j;
  vector<int> clause;
  int flag;

  for(i = 0; i<formula.size();i++){
      clause = formula[i];
      flag = 0;
      for(j = 0;j<clause.size();j++){
          if(clause[j] > 0 && assignment[abs(clause[j])] == 1){
              flag++;
              break;
          }
          if(clause[j] < 0 && assignment[abs(clause[j])] == 0){
              flag++;
              break;
          }
      }
      if(flag){
        result++;
      }
  }
  return result;

}


vector< vector <int> > genClausesMaxSatFlipCounter(vector<int> assignment,vector< vector <int> > formula,int k,int* var){
  vector<int> auxClause;
  unsigned int i,j;
  int oldClausesNum;
  int newClausesNum;
  int clausesTrue,maxClausesFalse;
  int addedClausesMax,addedClausesFlip;
  int numClausesAux;

  clausesTrue = getNumClausesTrue(formula,assignment);
  maxClausesFalse = formula.size() - clausesTrue - 1;

  //cout << "clausulas ja satifeitas: " << clausesTrue << endl;
  //cout << "max de clausulas falsas na instancia: " << maxClausesFalse << endl;


  vector< vector <int> > clausesFlipRestriction;
  vector< vector <int> > clausesMaxRestriction;

  //cout << "fazendo clauses constraint flip" << endl;

  //gen clauses
  //clause 1
  oldClausesNum = formula.size();

  auxClause.push_back(valueVar(1,assignment));
  auxClause.push_back(*var + 1 + getIndex(1,1,k));

  clausesFlipRestriction.push_back(auxClause);

  //clase 2
  for(j = 2; j<= k;j++){
    auxClause.clear();
    auxClause.push_back(-1 * (*var + 1 + getIndex(1,j,k)));
    clausesFlipRestriction.push_back(auxClause);
  }

  //clause 3

  for(i = 2;i < *var;i++){
    auxClause.clear();
    auxClause.push_back(valueVar(i,assignment));
    auxClause.push_back(*var + 1 + getIndex(i,1,k));
    clausesFlipRestriction.push_back(auxClause);
    auxClause.clear();
    auxClause.push_back(-1 * (*var + 1 + getIndex(i - 1,1,k)));
    auxClause.push_back(*var + 1 + getIndex(i,1,k));
    clausesFlipRestriction.push_back(auxClause);
    for(j = 2;j<=k;j++){
      auxClause.clear();
      auxClause.push_back(valueVar(i,assignment));
      auxClause.push_back(-1 * (*var + 1 + getIndex(i - 1,j-1,k)));
      auxClause.push_back(*var + 1 + getIndex(i,j,k));
      clausesFlipRestriction.push_back(auxClause);
      auxClause.clear();
      auxClause.push_back(-1 * (*var + 1 + getIndex(i - 1,j,k)));
      auxClause.push_back(*var + 1 + getIndex(i,j,k));
      clausesFlipRestriction.push_back(auxClause);
    }
    auxClause.clear();
    auxClause.push_back(valueVar(i,assignment));
    auxClause.push_back(-1 * (*var + 1 + getIndex(i - 1,k,k)));
    clausesFlipRestriction.push_back(auxClause);
  }
  auxClause.clear();
  auxClause.push_back(valueVar(*var,assignment));
  auxClause.push_back(-1 * (*var + 1 + getIndex(*var - 1,k,k)));
  clausesFlipRestriction.push_back(auxClause);

  addedClausesFlip = clausesFlipRestriction.size();

  //*************************************************************
  //contruct max restrictions
  numClausesAux = 0;

  //cout << "fazendo clauses constraint max" << endl;

  int offset = (*var + (*var * k) - k) + 1;

  int n = formula.size();
  int k2 = maxClausesFalse;

  auxClause.clear();
  auxClause = formula[0];
  auxClause.push_back(offset + getIndex(1,1,k2));

  clausesMaxRestriction.push_back(auxClause);
  numClausesAux++;

  //clase 2
  for(j = 2; j<= k2;j++){
    auxClause.clear();
    auxClause.push_back(-1 * (offset + getIndex(1,j,k2)));
    clausesMaxRestriction.push_back(auxClause);
    numClausesAux++;

    //cout << "1-> " << numClausesAux << endl;
  }

  //clause 3

  for(i = 2;i < n;i++){
    auxClause.clear();
    //cout << "aqui i = " << i << endl;
    auxClause = formula[i - 1];
    //cout << "aqui2" << endl;
    auxClause.push_back(offset + getIndex(i,1,k2));
    clausesMaxRestriction.push_back(auxClause);
    numClausesAux++;
    //cout << "2-> " << numClausesAux << endl;




    auxClause.clear();
    auxClause.push_back(-1 * (offset + getIndex(i - 1,1,k2)));
    auxClause.push_back(offset + getIndex(i,1,k2));
    clausesMaxRestriction.push_back(auxClause);
    numClausesAux++;

    //cout << "3-> " << numClausesAux << endl;
    for(j = 2;j<=k2;j++){
      auxClause.clear();
      auxClause = formula[i - 1];
      auxClause.push_back(-1 * (offset + getIndex(i - 1,j-1,k2)));
      auxClause.push_back(offset + getIndex(i,j,k2));
      clausesMaxRestriction.push_back(auxClause);
      numClausesAux++;
      //cout << "4-> " << numClausesAux << endl;

      auxClause.clear();
      auxClause.push_back(-1 * (offset + getIndex(i - 1,j,k2)));
      auxClause.push_back(offset + getIndex(i,j,k2));
      clausesMaxRestriction.push_back(auxClause);
      numClausesAux++;

      //cout << "5-> " << numClausesAux << endl;
    }
    auxClause.clear();
    auxClause = formula[i - 1];
    auxClause.push_back(-1 * (offset + getIndex(i - 1,k2,k2)));
    clausesMaxRestriction.push_back(auxClause);
    numClausesAux++;
    //cout << "6-> " << numClausesAux << endl;

  }
  //cout << "aqui3" << endl;
  auxClause.clear();
  auxClause = formula[n - 1];
  auxClause.push_back(-1 * (offset + getIndex(n - 1,k2,k2)));
  clausesMaxRestriction.push_back(auxClause);
  numClausesAux++;
  //cout << "7-> " << numClausesAux << endl;


  addedClausesMax = clausesMaxRestriction.size();


  //end max restrictions
  //*************************************************************

  //cout << "clauses antes: " << oldClausesNum << " clauses depois: " << (addedClausesMax + addedClausesFlip) << endl;
  //cout << "variaveis antes: " << *var << " variaveis depois: " << ((*var + (*var * k) - k) + (n * k2) - k2) << endl;
  *var = ((*var + (*var * k) - k) + (n * k2) - k2);


  //cout << "ajeitando formula final" << endl;
  //ajeitar formula final
  formula.clear();
  formula = clausesFlipRestriction;
  formula.insert(end(formula), begin(clausesMaxRestriction), end(clausesMaxRestriction));

  return formula;
}

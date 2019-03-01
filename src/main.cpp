#include "parser.hpp"
#include "solve.hpp"

using namespace std;

int main(int argc, char** argv){
  int k;
  vector< vector <int>> formula;
  vector< vector <int>> formulaFlip;
  vector<int> assignment;
  int numVariables;
  int numClauses;
  unsigned int i;
  int auxi;

  if(argc != 3){
    cout << "Usage: ./exe <.cnf dimacs file> k" << endl;
    cout << "The program generates an assignment of the cnf formula and tries to improve the assignment by fliping at most k variables" << endl;
    return 1;
  }

  string dimacs(argv[1]);
  k = atoi(argv[2]);

  formula = parseIn(dimacs,&numVariables,&numClauses);

  assignment = genAssignment(numVariables);
  //formulaFlip = genClausesSatFlip(assignment, formula, k,numVariables);
  formulaFlip = genClausesSatFlipCounter( assignment, formula, k,&numVariables);

  string outName("outputs/outFlip");

  parseOut(formulaFlip,numVariables,outName);

  cout << "Given an assignment: " << endl;
  for(i = 1;i < assignment.size();i++){
    //cout << "value" << assignment[i] << endl;
      if(assignment[i] == 0){
          auxi = i * -1;
          cout << " " <<  auxi;
      }else{
          cout << " " << i;
      }
  }
  cout << endl;

  cout << "Is is possible to satisfy the given formula flipping at most " << k << " variables at the assignment?" << endl;

  system("./glucose_static -verb=0 outputs/outFlip outputs/outFlipResult");

  ifstream out("outputs/outFlipResult");
  if(!out.is_open()){
      cout << "Error opening output of glucose" << endl;
  }else{
      cout << out.rdbuf();
  }

  return 0;
}

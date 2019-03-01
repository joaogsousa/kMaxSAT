#include "parser.hpp"
#include "solve.hpp"

using namespace std;

int main(int argc, char** argv){
  int k;
  vector< vector <int>> formula;
  vector< vector <int>> formulaFlip;
  vector<int> assignment;
  int numVariables, numVariablesOriginal;
  int numClauses;
  unsigned int i;
  int auxi;
  int clausesTrueBefore;
  int calusesTrueAfter;
  int literal;

  if(argc != 3){
    cout << "Usage: ./exe <.cnf dimacs file> k" << endl;
    cout << "The program generates an assignment of the cnf formula and tries to improve the assignment by fliping at most k variables" << endl;
    return 1;
  }

  string dimacs(argv[1]);
  k = atoi(argv[2]);

  formula = parseIn(dimacs,&numVariables,&numClauses);
  numVariablesOriginal = numVariables;

  assignment = genAssignment(numVariables);




  //k-flip sat combinatorio
  //formulaFlip = genClausesSatFlip(assignment, formula, k,numVariables);

  //k-flip sat counter
  //formulaFlip = genClausesSatFlipCounter( assignment, formula, k,&numVariables);

  int end = 0;
  int inicio = 1;

  while(!end){

    numVariables = numVariablesOriginal;
    clausesTrueBefore = getNumClausesTrue( formula, assignment);

    if(inicio){
      system("clear");
      cout << "**************************************************" << endl;
      cout << "               K-flip MAX SAT solver              " << endl;
      cout << "**************************************************" << endl;
      cout << "Given a initial random assignment: " << endl;
    }else{
      system("clear");
      cout << "Given the current constructed assignment: " << endl;
    }
    inicio = 0;

    for(i = 1;i < assignment.size();i++){
      //cout << "value" << assignment[i] << endl;
        if(assignment[i] == 0){
            auxi = i * -1;
            cout << " " <<  auxi;
        }else{
            cout << " " << i;
        }
    }
    cout << endl << endl;

    if(clausesTrueBefore == numClauses){
      cout << "Current assignment already satisfies all clauses." << endl;
      exit(1);
    }else if(clausesTrueBefore == numClauses - 1){
      cout << "Current assignment already satisfies all clauses minus 1." << endl;
      cout << "->Constructing the k-flip SAT encoding to solve the problem..." << endl;

      //k-flip sat counter
      formulaFlip = genClausesSatFlipCounter( assignment, formula, k,&numVariables);
    }else{
      //k-flip max sat
      cout << "->Constructing the k-flip max SAT encoding to solve the problem..." << endl;
      formulaFlip = genClausesMaxSatFlipCounter( assignment, formula, k,&numVariables);

    }


    string outName("outputs/outFlip");

    parseOut(formulaFlip,numVariables,outName);

    cout << endl;
    cout << "Is it possible to satisfy more than " << clausesTrueBefore << " clauses of the given formula by flipping at most " << k << " variables of the current assignment?" << endl;
    cout << "->Solving the constructed SAT encoding..." << endl << endl;

    system("./glucose_static -verb=0 outputs/outFlip outputs/outFlipResult > /dev/null");

    ifstream out("outputs/outFlipResult");

    if(!out.is_open()){
        cout << "Error opening output of glucose" << endl;
        exit(1);
    }

    char c;
    out.get(c);
    if(c == 'U'){
      cout << "UNSATISFIABLE, imposible to improve assignment" << endl;
      end = 1;
    }else{
      out.unget();
      //cout << "new assignment: " << endl;
      for(i = 1; i <= numVariablesOriginal;i++){
        out >> literal;
        if(literal > 0){
            assignment[i] = 1;
        }else{
            assignment[i] = 0;
        }
        //cout << " " << literal;
      }


      cout << "SATISFIABLE, SAT encoding solution:" << endl;
      out.clear();
      out.seekg(0, ios::beg);
      cout << out.rdbuf();

      cout << endl << "Press <enter> for the next iteration." << endl;
      cin.get();
    }

  }



  return 0;
}

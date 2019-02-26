#include "parser.hpp"

using namespace std;

int main(int argc, char** argv){
  int k;
  vector< vector <int>> formula;

  if(argc != 3){
    cout << "Usage: ./exe <.cnf dimacs file> k" << endl;
    cout << "The program generates an assignment of the cnf formula and tries to improve the assignment by fliping at most k variables" << endl;
    return 1;
  }

  string dimacs(argv[1]);
  k = atoi(argv[2]);

  formula = parseIn(dimacs);
  string outName("outputs/out1");

  parseOut(formula,6,outName);



  return 0;
}

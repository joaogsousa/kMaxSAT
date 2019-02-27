#include "parser.hpp"

using namespace std;

void printFormula(vector< vector <int> > formula){
  unsigned int i,j;
  for(i = 0;i < formula.size();i++){
    cout << "clause " << i <<":" << endl;
    cout << "(";
    for(j = 0;j < formula[i].size();j++){
      cout << formula[i][j] << ",";
    }
    cout << ")" << endl;

  }
}


vector< vector <int> > parseIn(string input,int * numVariables, int * numClauses){
    string line,str1,str2;
    ifstream cnf;
    char type;
    int numVariablesAux,numClausesAux;
    int i,num;
    vector <int> vecAux;

    vector< vector <int> > result;

    try{
      cnf.open(input);


      if(!cnf.is_open()){
        cout << "Error receiving input CNF" << endl;
        exit(1);
      }

    }catch(const exception& e){
      cout << e.what() << endl;
    }



    while(cnf >> type){
      if(type == 'c'){
        getline(cnf,line);
          //comment line
      }else if(type == 'p'){
        //problem line
        cnf >> str2 >> numVariablesAux >> numClausesAux;
        *numVariables = numVariablesAux;
        *numClauses = numClausesAux;
        break;
      }
    }

    i = 0;
    result.push_back(vecAux);
    while(cnf >> num){
      if(num == 0){
          i++;
          result.push_back(vecAux);
      }else{
        result[i].push_back(num);
      }
    }

    result.pop_back();


    return result;


}

void parseOut(vector< vector <int> > formula,int var,string output){
  ofstream dimacs(output);

  if(!dimacs.is_open()){
    cout << "error opening output file" << endl;
    exit(1);
  }

  dimacs << "p cnf " << var << " " << formula.size() << endl;

  unsigned int i,j;

  for(i = 0;i < formula.size();i++){
    for(j = 0;j < formula[i].size();j++){
      dimacs << formula[i][j] << " ";
    }
    dimacs << "0" << endl;

  }

  dimacs.close();


}

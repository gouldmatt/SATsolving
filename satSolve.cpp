#include <iostream> 
#include <fstream> 
#include <vector> 

using namespace std; 

int main(){
    vector<string> clauseVec; 
    string element = " "; 
    ifstream file; 
    file.open("satTest.CNF");
    int numVar = 0;
    int numClause = 0; 
    

    // read entire file into vector keeping only num 
    while(file >> element){
        if (element.find_first_not_of ("-0123456789") == string::npos){
            clauseVec.push_back(element);
        }
    }

    // first element is the num of variables 
    numVar = stoi(clauseVec[0]);
    // second is num of clauses 
    numClause = stoi(clauseVec[1]);
    //clauseVec.erase(clauseVec.begin(),clauseVec.begin()+2); 
    
    for(int i = 0; i < clauseVec.size(); i++){
        cout << clauseVec[i] << endl; 
    }

    cout << "clause vec size: " << clauseVec.size() << endl; 




    return 0; 
}
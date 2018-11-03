#include <iostream> 
#include <fstream> 
#include <vector> 
#include <bitset> 

using namespace std; 

vector<int> satTest(vector<int>& clauseVec);
bool checkSol(bitset <1024>& solutionData, vector<int>& clauseVec);

int main(){
    vector<int> clauseVec; 
    vector<int> solVec;
    string element = " "; 
    ifstream file; 
    file.open("satTest.CNF");
    int numVar = 0;
    int numClause = 0; 
    

    // read entire file into vector keeping only num 
    while(file >> element){
        if (element.find_first_not_of ("-0123456789") == string::npos){
            clauseVec.push_back(stoi(element));
        }
    }

    solVec = satTest(clauseVec); 

    for(int i=0; i<solVec.size(); i++){
        cout << solVec[i] << " "; 
    }


    return 0; 
}

vector<int> satTest(vector<int>& clauseVec){
    int numVar = clauseVec[0];
    int backTrackNum = 1; 
    vector<int> solutionVec;
    bitset <1024> solutionData (numVar); 
  
    solutionVec.reserve(numVar); 

    while(checkSol(solutionData,clauseVec) == false){
        if(backTrackNum > numVar){
           // equation is not satisfiable 
           return solutionVec; 
        }
        solutionData = bitset<1024> (numVar - backTrackNum); 
    }

    // construct solution vector 
    for(int i=0; i<numVar; i++){
        if(solutionData[i] == 1){
            solutionVec[i] = i; 
        } else {
            solutionVec[i] = -i; 
        } 
    }
    return solutionVec; 
}

bool checkSol(bitset <1024>& solutionData, vector<int>& clauseVec){
    // for each clause check solution data
    /* 
    for(int i=1; i<clauseVec.size(); i++){
        if(clauseVec[i] != 0){
            if(clauseVec[i] > 0){
                if(solutionData[clauseVec[i]] == 1){
                    return true; 
                } 
            }else{
                if(solutionData[clauseVec[i]].flip() == 1){
                    return true; 
                } 
            }
        }
    }
    */


    return false; 
}
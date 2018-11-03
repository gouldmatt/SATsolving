#include <iostream> 
#include <fstream> 
#include <vector> 
#include <bitset> 

using namespace std; 

void satTest(vector<int>& clauseVec, vector<int>& solutionVec);
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

    satTest(clauseVec,solVec); 

    for(int i=0; i<clauseVec[0]; i++){
        cout << solVec[i] << " "; 
    }


    return 0; 
}

void satTest(vector<int>& clauseVec,vector<int>& solutionVec){
    int numVar = clauseVec[0];
    int backTrackNum = 1; 
    bitset <1024> solutionData (numVar); 
  
    solutionVec.reserve(numVar); 

    while(checkSol(solutionData,clauseVec) == false){
        if(backTrackNum > numVar){
           // equation is not satisfiable 
           cout << "not satisfiable"; 
           return; 
        }
        solutionData = bitset<1024> (numVar - backTrackNum); 
    }

    cout << "numVar: " << numVar << endl; 
    // construct solution vector 
    for(int i=0; i<numVar; i++){
        if(solutionData.test(i)){
            solutionVec[i] = i; 
        } else {
            solutionVec[i] = -i; 
        } 
    }
    return; 
}

bool checkSol(bitset <1024>& solutionData, vector<int>& clauseVec){
    // for each clause check solution data
     
    for(int i=1; i<clauseVec.size(); i++){
        if(clauseVec[i] != 0){
            if(clauseVec[i] > 0){
                if(solutionData.test(clauseVec[i])){
                    return true; 
                } 
            }else{
                if(solutionData.test(-clauseVec[i])){
                    return true; 
                } 
            }

        }
    }

    return false; 
}
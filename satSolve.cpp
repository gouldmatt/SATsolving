#include <iostream> 
#include <fstream> 
#include <vector> 
#include <bitset> 
#include <cmath> 

using namespace std; 

void satTest(vector<int>& clauseVec, vector<int>& solutionVec);
bool checkSol(bitset <1024>& solutionData, vector<int>& clauseVec);

int main(){
    vector<int> clauseVec; 
    vector<int> solVec;
    string element = " "; 
    ifstream file; 
    file.open("satTest3.CNF");
    int numVar = 0;
    int numClause = 0; 
    

    // read entire file into vector keeping only num 
    while(file >> element){
        if (element.find_first_not_of ("-0123456789") == string::npos){
            clauseVec.push_back(stoi(element));
            cout << element;
        }
    }
    cout << endl << endl; 
    satTest(clauseVec,solVec); 
    cout << endl << endl; 
    if(solVec.size() != 0){
        for(int i=0; i<solVec.size(); i++){
            cout << solVec[i]+1 << " "; 
        }
    }

    return 0; 
}

void satTest(vector<int>& clauseVec,vector<int>& solutionVec){
    int numVar = clauseVec[0];
    int backTrackNum = 1;  
    bitset <1024> solutionData (pow(2,numVar)-1); 
  
    solutionVec.reserve(numVar); 

    while(checkSol(solutionData,clauseVec) == false){
        if(backTrackNum > numVar){
           // equation is not satisfiable 
           cout << "not satisfiable"; 
           return; 
        }
        solutionData = bitset<1024> (pow(2,(numVar - backTrackNum))-1); 
        cout << "backtrack: " << backTrackNum << endl; 
        backTrackNum++; 
    }

    cout << "numVar: " << numVar << endl; 
    // construct solution vector 
    cout << solutionData; 
    for(int i=0; i<numVar; i++){
        if(solutionData.test(i)){
            solutionVec.push_back(i); 
        } else {
            solutionVec.push_back(-i); 
        } 
    }
    return; 
}

bool checkSol(bitset <1024>& solutionData, vector<int>& clauseVec){
    int numClause = 0; 
    int index = 2; 
    cout << solutionData << endl; 

    // for each clause check solution data
    while(numClause < clauseVec[1]){
        if(clauseVec[index] > 0){
            if(solutionData.test(clauseVec[index]-1)){
                while(clauseVec[index] != 0){
                 
                    index++;       
                }
                numClause++; 
                cout << endl << "index " << index << endl; 
                cout << "clause # " << numClause << endl; 
                if(numClause == clauseVec[1]){
                    return true; 
                }
            }
        } else if (clauseVec[index] < 0){
            if(solutionData.test(-(clauseVec[index]-1)) == false){
                while(clauseVec[index] != 0){
                    index++;       
                }
                numClause++; 
                cout << endl << "index " << index << endl; 
                cout << "clause # " << numClause << endl; 
                if(numClause == clauseVec[1]){
                    return true; 
                }
                
            }
        } else {
            numClause++; 
            if(numClause == clauseVec[1]){
                return false; 
            }
        }
        index++;
    }
    
    return true; 
}
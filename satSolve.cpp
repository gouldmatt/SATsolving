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
    file.open("tt1.CNF");
    int numVar = 0;
    int numClause = 0; 
    

    // read entire file into vector keeping only num 
    while(file >> element){
        if (element.find_first_not_of ("-0123456789") == string::npos){
            clauseVec.push_back(stoi(element));
        }
    }
    
    satTest(clauseVec,solVec); 
   
    if(solVec.size() != 0){
        cout << "v ";
        for(int i=0; i<solVec.size(); i++){
            cout << solVec[i] << " "; 
        }
        cout << "0" << endl; 
    }

    return 0; 
}

void satTest(vector<int>& clauseVec,vector<int>& solutionVec){
    int numVar = clauseVec[0];
    long int numPosSol = pow(2,numVar);
    long int backTrackNum = 1;  
    bitset <1024> solutionData (pow(2,numVar)-1); 
  
    solutionVec.reserve(numVar); 

    while(checkSol(solutionData,clauseVec) == false){
        if(backTrackNum > numPosSol){
           // equation is not satisfiable 
           cout << "not satisfiable" << endl; 
            
           return; 
        }
        solutionData = bitset<1024> (pow(2,(numVar))-1-backTrackNum); 
        //cout << "backtrack: " << backTrackNum << endl; 
        
        backTrackNum++; 
    }

    // construct solution vector 
    for(int i=1; i<=numVar; i++){
        if(solutionData.test(i-1)){
            solutionVec.push_back(i);
        } else {
            solutionVec.push_back(-i);
        }
    }
    return; 
}


bool checkSol(bitset <1024>& solutionData, vector<int>& clauseVec){
    int clauseNum = 0;  
    int clauseOffset = 2; // start at 2 because of # of clauses /var

    while(clauseOffset < clauseVec.size()){
       if(clauseVec[clauseOffset] > 0){
            if(solutionData.test(clauseVec[clauseOffset]-1)){
                //cout << "pos" << endl; 
                //cout << "clauseVec value: " << clauseVec[clauseOffset]-1 << endl; 
                //cout << "clauseNum: " << clauseNum << endl;
                //cout << "clauseOffset: " << clauseOffset << endl << endl; 
                // move offset now that clause is sat 
                while(clauseVec[clauseOffset] != 0){
                    clauseOffset++;       
                }
                clauseOffset++;
                clauseNum++; 
                continue;  
            }
        } else if (clauseVec[clauseOffset] < 0){
            if(solutionData.test((-clauseVec[clauseOffset])-1) == false){
                //cout << "neg" << endl; 
                //cout << "clauseVec value: " << clauseVec[clauseOffset]-1 << endl; 
                //cout << "clauseNum: " << clauseNum << endl;
                //cout << "clauseOffset: " << clauseOffset << endl << endl; 
                // move offset now that clause is sat 
                while(clauseVec[clauseOffset] != 0){
                    clauseOffset++;       
                }
                clauseOffset++;
                clauseNum++; 
                continue;  
            }
        } else {
            //cout << "failed to satisfy clause element: " << clauseVec[clauseOffset] << endl << endl; 
            return false;  
        }
        //cout << clauseOffset; 
        if(clauseOffset == clauseVec[1]){
            return false; 
        }
        clauseOffset++; 
    }
    return true; 
}
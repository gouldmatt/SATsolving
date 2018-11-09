#include <iostream> 
#include <fstream> 
#include <vector> 
#include <bitset> 
#include <cmath> 
#include <thread>
#include <mutex> 

using namespace std; 

std::mutex mtx;

void satTest(vector<int>& clauseVec, vector<int>& solutionVec,int threadNumber);
bool checkSol(bitset <1024>& solutionData, vector<int>& clauseVec);

int main(){
    thread threads[4];
    vector<int> clauseVec; 
    vector<int> solVec[4];
    string element = " "; 
    ifstream file; 
    file.open("satTest.CNF");
    

    // read entire file into vector keeping only num 
    while(file >> element){
        if (element.find_first_not_of ("-0123456789") == string::npos){
            clauseVec.push_back(stoi(element));
        }
    }
    
    // start multithreading
    for(int i=0; i<4; i++){ 
       threads[i] = thread(satTest,std::ref(clauseVec),std::ref(solVec[i]),i); 
    }

    // stop multithreading
    for(int i=0; i<4; i++){
        threads[i].join();
    }
    
    for(int j=0; j < 4; j++){
        if(solVec[j].size() != 0){
            cout << "thread: " << j << " says satisfiable with: v ";
            for(int i=0; i<solVec[j].size(); i++){
            cout << solVec[j][i] << " "; 
            }
            cout << "0" << endl; 
        }
    }
    
    return 0; 
}

void satTest(vector<int>& clauseVec,vector<int>& solutionVec,int threadNumber){
    int numVar = clauseVec[0];
    long int numPosSol = pow(2,numVar-2);
    long int backTrackNum = 1; 
    long int backTrackTrack = 0; 
    bitset <1024> solutionData (numPosSol-1); 

    if(numVar > 1){
        if(threadNumber == 0 || threadNumber == 1){
            solutionData[numVar-1] = 0;
            solutionData[numVar-2] = threadNumber; 
        } else {
            solutionData[numVar-1] = 1;
            solutionData[numVar-2] = threadNumber - 3;  
        }
    } else {
        numPosSol = pow(2,numVar); 
    }

    /*
    mtx.lock(); 
    cout << "thread Number: " << threadNumber << endl; 

    cout << "solution guess: ";
    for(int k=0; k<numVar; k++){
        cout << solutionData[k];
    }
    cout << endl; 
   
    mtx.unlock();
    */

    solutionVec.reserve(numVar); 
    
    while(checkSol(solutionData,clauseVec) == false){
        if(backTrackNum > numPosSol){
           // equation is not satisfiable
           mtx.lock(); 
           cout << "thread: " << threadNumber << " says not satisfiable" << endl; 
           mtx.unlock(); 
            
           return; 
        }
        solutionData = bitset<1024> (solutionData.to_ulong()-backTrackNum); 
        mtx.lock();
        backTrackTrack++;
        if(backTrackTrack > 1000000){
            cout << "backtrack: " << backTrackNum << endl; 
            backTrackTrack = 0; 
        }
        mtx.unlock(); 
        
        backTrackNum++; 
    }
    
    /*
    mtx.lock();
    cout << "found sol" << endl; 
    mtx.unlock(); 
    */

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
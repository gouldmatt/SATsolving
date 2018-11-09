#include <iostream> 
#include <fstream> 
#include <vector> 
#include <bitset> 
#include <cmath> 
#include <thread>
#include <mutex> 
#include <time.h> 

using namespace std; 

std::mutex mtx;

void satTest(vector<int>& clauseVec, vector<int>& solutionVec, int threadNumber, bool& foundSol);
bool checkSol(bitset <1024>& solutionData, vector<int>& clauseVec);

int main(){
    thread threads[4];
    vector<int> clauseVec; 
    vector<int> solVec[4];
    string element = " "; 
    ifstream file; 
    file.open("satTest5.CNF");
    bool foundSol; 
    int i=0; 
    

    // read entire file into vector keeping only num 
    while(file >> element){
        if (element.find_first_not_of ("-0123456789") == string::npos){
            clauseVec.push_back(stoi(element));
        }
    }
    
    if(clauseVec[0] > 3){
        cout << "multithreading" << endl; 
        // start multithreading
        for(i=0; i<4; i++){ 
        threads[i] = thread(satTest,std::ref(clauseVec),std::ref(solVec[i]),i,std::ref(foundSol)); 
        }

        // stop multithreading
        for(i=0; i<4; i++){
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
        cout << endl << endl; 

    } else {
        cout << "single" << endl; 

        satTest(clauseVec,solVec[0],i=0,foundSol);
        if(solVec[0].size() != 0){
                cout << "satisfiable: v "; 
                for(int i=0; i<solVec[0].size(); i++){
                    cout << solVec[0][i] << " "; 
                }
                cout << "0" << endl; 
        }
        cout << endl << endl; 

    }
    
    return 0; 
}

void satTest(vector<int>& clauseVec, vector<int>& solutionVec, int threadNumber, bool& foundSol){
    int numVar = clauseVec[0];
    //remove 2 variables in possible solution calc so that thread stops after checking its solution space 
    long int numPosSol = pow(2,numVar-2);
    long int backTrackNum = 1; 
    bitset <1024> solutionData (numPosSol-1); 
    time_t previousBacktrackPrint = time(&previousBacktrackPrint); 
    time_t currentBacktrackTime;
    double timeSincePrint = 0.0; 

    if(numVar > 3){
        // set the first 2 variables based on thread number 
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

        if(foundSol == true){       
            return; 
        }
    

        time(&currentBacktrackTime);

        timeSincePrint = difftime(currentBacktrackTime,previousBacktrackPrint);
        
        if(timeSincePrint > 2){
            mtx.lock();
            cout << "thread # " << threadNumber << " backtrack number: " << backTrackNum << endl; 
            mtx.unlock();
            time(&previousBacktrackPrint); 
        }
     
        
        backTrackNum++; 
    }
    

    mtx.lock();
    cout << "found sol" << endl; 
    foundSol = true; 
    mtx.unlock(); 

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
    int totalOffset = 2; // start at 2 because of # of clauses /var

    while(totalOffset < clauseVec.size()){
       if(clauseVec[totalOffset] > 0){
            if(solutionData.test(clauseVec[totalOffset]-1)){
                //cout << "pos" << endl; 
                //cout << "clauseVec value: " << clauseVec[totalOffset]-1 << endl; 
                //cout << "clauseNum: " << clauseNum << endl;
                //cout << "totalOffset: " << totalOffset << endl << endl; 
                // move offset now that clause is sat 
                while(clauseVec[totalOffset] != 0){
                    totalOffset++;       
                }
                totalOffset++;
                clauseNum++; 
                continue;  
            }
        } else if (clauseVec[totalOffset] < 0){
            if(solutionData.test((-clauseVec[totalOffset])-1) == false){
                //cout << "neg" << endl; 
                //cout << "clauseVec value: " << clauseVec[totalOffset]-1 << endl; 
                //cout << "clauseNum: " << clauseNum << endl;
                //cout << "totalOffset: " << totalOffset << endl << endl; 
                // move offset now that clause is sat 
                while(clauseVec[totalOffset] != 0){
                    totalOffset++;       
                }
                totalOffset++;
                clauseNum++; 
                continue;  
            }
        } else {
            //cout << "failed to satisfy clause element: " << clauseVec[totalOffset] << endl << endl; 
            return false;  
        }
        //cout << clauseOffset; 
        if(totalOffset == clauseVec[1]){
            return false; 
        }
        totalOffset++; 
    }
    return true; 
}
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

long int t0BackTrack;
long int t1BackTrack;
long int t2BackTrack;
long int t3BackTrack;
bool timerOn = true;


void satTest(vector<int>& clauseVec, vector<int>& solutionVec, int threadNumber, bool& foundSol);
bool checkSol(bitset <1024>& solutionData, vector<int>& clauseVec);
void printBackTracks(long int& t0BackTrack, 
    long int& t1BackTrack, long int& t2BackTrack, long int& t3BackTrack);

int main(int argc, char* argv[]){

    thread threads[4];
    vector<int> clauseVec; 
    vector<int> solVec[4];
    string element = " "; 
    ifstream file; 
    bool foundSol; 
    int i=0; 
    
    //Read in file from command line when running
    if(argc < 2){
        cerr << "Must run with a file: ./output <file.CNF>\n";
        return 1;
    }

    string fileName = argv[1];
    file.open(fileName);

    // read entire file into vector keeping only num 
    while(file >> element){
        if (element.find_first_not_of ("-0123456789") == string::npos){
            clauseVec.push_back(stoi(element));
        }
    }

    // only multithread if there is enough variables 
    if(clauseVec[0] > 3){
        cout << "multithreading" << endl; 
        // start multithreading
        for(i=0; i<4; i++){ 
            threads[i] = thread(satTest,std::ref(clauseVec),std::ref(solVec[i]),i,std::ref(foundSol)); 
        }

        //start a thread to print number of backtracks to date every two seconds
        thread timeThread(printBackTracks, 
            std::ref(t0BackTrack), std::ref(t1BackTrack),
            std::ref(t2BackTrack), std::ref(t3BackTrack));
            
        // stop multithreading
        for(i=0; i<4; i++){
            threads[i].join();
        }
        
        timeThread.join();
        
        for(int j=0; j < 4; j++){
            if(solVec[j].size() != 0){
                cout << "thread " << j << " says satisfiable with: v ";
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

    if(numVar > 3){
        // set the first 2 variables based on thread number 
        if(threadNumber == 0 || threadNumber == 1){
            solutionData[numVar-1] = 0;
            solutionData[numVar-2] = threadNumber; 
            
        } else {
            solutionData[numVar-1] = 1;
            solutionData[numVar-2] = abs(threadNumber - 3); 
        }
    } else {
        numPosSol = pow(2,numVar); 
    }
    
    solutionVec.reserve(numVar); 
    
    while(checkSol(solutionData,clauseVec) == false){
        cout << backTrackNum << endl;
        if(backTrackNum > numPosSol){
            mtx.lock(); 
            cout << "backtrack num: " << backTrackNum << " posSol: " << numPosSol << endl;
            cout << "thread: " << threadNumber << " says not satisfiable" << endl; 
            timerOn = false;
            mtx.unlock(); 
            
           return; 
        }
        solutionData = bitset<1024> (solutionData.to_ulong()- 1); 
        
        if(foundSol == true){        
            return; 
        }
    
        //Check thread ID and update globals
        if(threadNumber == 0){
            t0BackTrack = backTrackNum;
        }

        if(threadNumber == 1){
            t1BackTrack = backTrackNum;
        }

        if(threadNumber == 2){
            t2BackTrack = backTrackNum;
        }

        if(threadNumber == 3){
            t3BackTrack = backTrackNum;
        }
        
        backTrackNum++; 
    }
  
    mtx.lock();
    foundSol = true;
    timerOn = false;  
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
            
                // move offset now that clause is sat 
                while(clauseVec[totalOffset] != 0){
                    totalOffset++;       
                }
                totalOffset++;
                clauseNum++; 
                continue;  
            }
        } else { 
            return false;  
        } 
        if(clauseNum == clauseVec[1]){
            return false; 
        }
        totalOffset++; 
    }
    return true; 
}

void printBackTracks(long int& t0BackTrack, long int& t1BackTrack, long int& t2BackTrack,
    long int& t3BackTrack){
    //Prints number of backtracks to date every 2 seconds
    time_t previousBacktrackPrint = time(&previousBacktrackPrint); 
    time_t currentBacktrackTime;
    double timeSincePrint = 0.0;
    long int backTracksTotal = 0;

    while(timerOn == true){
        time(&currentBacktrackTime);

        timeSincePrint = difftime(currentBacktrackTime,previousBacktrackPrint);
        
        if(timeSincePrint >= 2){
            backTracksTotal = t0BackTrack + t1BackTrack + t2BackTrack +t3BackTrack;
            cout << "Number of backtracks: " << backTracksTotal << endl; 
            
            time(&previousBacktrackPrint); 
        }
    } 
}

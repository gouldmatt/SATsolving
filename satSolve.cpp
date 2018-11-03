#include <iostream> 
#include <fstream> 
#include <vector> 

using namespace std; 

int main(){
    vector<string> elementVector; 
    string element = " "; 
    ifstream file; 
    file.open("satTest.CNF");

    while(file >> element){
        if(element.front() != 'c' || element.front() != 'p'){
            continue; 
        }
        elementVector.push_back(element);
        cout << element << endl; 
        
    }




    return 0; 
}
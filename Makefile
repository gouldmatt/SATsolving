#target: dependencies
#	action

CFLAGS  = -Wall 

output: satSolve.cpp
	g++ -pthread satSolve.cpp -g -std=c++11 -o output

clean:
	rm *.o output
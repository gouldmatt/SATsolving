#target: dependencies
#	action

CFLAGS  = -Wall 

output: satSolve.cpp
	g++ -std=c++11 -pthread satSolve.cpp -o output

clean:
	rm *.o output
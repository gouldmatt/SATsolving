#target: dependencies
#	action

CFLAGS  = -Wall 

output: satSolve.cpp
	g++ -pthread satSolve.cpp -o output

clean:
	rm *.o output
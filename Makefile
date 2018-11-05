#target: dependencies
#	action

CFLAGS  = -Wall 

output: satSolve.cpp
	g++ -pthread satSolve.cpp -o output
	time ./output

clean:
	rm *.o output
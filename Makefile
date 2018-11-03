#target: dependencies
#	action

CFLAGS  = -Wall 

output: satSolve.cpp
	g++ satSolve.cpp -o output
	./output

clean:
	rm *.o output
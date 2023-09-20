all: main.o 
	g++ -o TSP main.o
	 
main.o: main.cpp 
	g++ -c -g main.cpp -std=c++17 

clean: cleanobj cleanmain 

cleanobj:
	rm -f *.o

cleanmain:
	rm TSP

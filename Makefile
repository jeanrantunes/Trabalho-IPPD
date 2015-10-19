main: main.o
	g++ main.o -pthread

main.o: main.cpp
	g++ -c main.cpp -pthread

clean:
	rm *.o *.h.gch main

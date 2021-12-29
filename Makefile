ROGGY = -std=c++20 -masm=intel -fconcepts -pthread -fopenacc -fopenmp -ggdb3 -Wpedantic -Wall -Wextra -Wconversion -Winline -Wsign-conversion -Weffc++ -Wstrict-null-sentinel -Wold-style-cast -Wnoexcept -Wctor-dtor-privacy -Woverloaded-virtual -Wsign-promo -Wzero-as-null-pointer-constant -Wsuggest-final-types -Wsuggest-final-methods -Wsuggest-override
CC=g++
OUT=peton

all: peton

peton: main.cpp Machine.o IP_Machine.o Packet.o Link.o Interface.o types.o
	$(CC) $(ROGGY) $^ -o peton

Machine.o: Machine.cpp Machine.hpp 
	$(CC) $(ROGGY) -c $<

IP_Machine.o: IP_Machine.cpp IP_Machine.hpp
	$(CC) $(ROGGY) -c $<

Packet.o: Packet.cpp Packet.hpp
	$(CC) $(ROGGY) -c $<

Link.o: Link.cpp Link.hpp
	$(CC) $(ROGGY) -c $<

Interface.o: Interface.cpp Interface.hpp
	$(CC) $(ROGGY) -c $<

types.o: types.cpp types.hpp
	$(CC) $(ROGGY) -c $<

clean:
	rm *.o $(OUT)
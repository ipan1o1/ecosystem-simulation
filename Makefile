tests: test.o coord.o ensemble.o grille.o animal.o population.o jeu.o 
	clang++ -o tests test.o coord.o ensemble.o grille.o animal.o population.o jeu.o 

main: main.o coord.o ensemble.o grille.o animal.o population.o jeu.o
	clang++ -o main main.o coord.o ensemble.o grille.o animal.o population.o jeu.o

ensemble.o: ensemble.cpp ensemble.hpp
	clang++ -c ensemble.cpp

coord.o: coord.cpp coord.hpp
	clang++ -c coord.cpp

grille.o: grille.cpp grille.hpp
	clang++ -c grille.cpp

animal.o: animal.cpp animal.hpp
	clang++ -c animal.cpp

population.o: population.cpp population.hpp
	clang++ -c population.cpp

jeu.o: jeu.cpp jeu.hpp
	clang++ -c jeu.cpp

test.o: test.cpp
	clang++ -c test.cpp

main.o: main.cpp
	clang++ -c main.cpp

clean:
	rm -f *.o tests main

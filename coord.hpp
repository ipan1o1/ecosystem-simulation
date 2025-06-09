#ifndef COORD_HPP
#define COORD_HPP

#include <iostream>
#include <stdexcept>

using namespace std;

class Ensemble;

const int TAILLEGRILLE = 40;  // Taille fixe de la grille (40x40)

// Classe Coord représentant une position (lig, col) sur la grille.

class Coord {
    private:
        int lig;  // Ligne
        int col;  // Colonne

    public:

        //Constructeur à partir d'une ligne et d'une colonne.
        Coord(int lig, int col);

        // Renvoie la ligne de la coordonnée.
        int getLig() const;

        // Renvoie la colonne de la coordonnée.
        int getCol() const;

        /**
         * Renvoie un entier unique associé à cette coordonnée.
         * La formule est: lig * TAILLEGRILLE(40) + col
         */
        int toInt() const;

        // Constructeur à partir d'un entier encodé (entre 0 et TAILLEGRILLE*TAILLEGRILLE - 1).
        Coord(int value);

        // Renvoie l'ensemble des coordonnées voisines de cette coordonnée.
        
        Ensemble voisines() const;
};

 // Surcharge de l'opérateur d'affichage pour afficher une Coord au format (lig, col).
ostream& operator<<(ostream& out, const Coord& c);

 // Surcharge de l'opérateur de comparaison d'égalité.
bool operator==(const Coord& a, const Coord& b);

 // Surcharge de l'opérateur de comparaison de différence.
bool operator!=(const Coord& a, const Coord& b);

// Fonction pour créer une Coord à partir d’un entier.
Coord fromInt(int value);

#endif

#ifndef GRILLE_HPP
#define GRILLE_HPP

#include <iostream>
#include <stdexcept>
#include "ensemble.hpp"
#include "coord.hpp"

using namespace std;

const int VIDE = -1; // Constante représentant une case vide 

class Grille {
    private:
        int g[TAILLEGRILLE][TAILLEGRILLE]; // Grille de TAILLEGRILLE x TAILLEGRILLE contenant des identifiants ou VIDE

    public:
        Grille(); // Constructeur : initialise toutes les cases à VIDE

        bool caseVide(const Coord &c) const;      // Renvoie vrai si la case c est vide
        void setCase(const Coord &c, int id);     // Place un identifiant dans la case c
        void videCase(const Coord &c);            // Vide la case c 
        void afficher(ostream &os) const;         // Affiche la grille (version texte)
        int getCase(const Coord& c) const;        // Renvoie l'identifiant de la case c
};

#endif

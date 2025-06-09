#include <iostream>
#include <stdexcept>
#include "grille.hpp"
#include "doctest.h"
#include "ensemble.hpp"

using namespace std;

// Constructeur : initialise la grille avec des cases vides
Grille::Grille(){
    for(int i = 0; i < TAILLEGRILLE; i++){
        for (int j = 0; j < TAILLEGRILLE; j++){
            g[i][j] = VIDE;
        }
    }
}

// Renvoie vrai si la case donnée est vide
bool Grille::caseVide(const Coord &c) const {
    return g[c.getLig()][c.getCol()] == VIDE;
}

// Place un identifiant dans la case donnée
void Grille::setCase(const Coord& c, int id) {
    g[c.getLig()][c.getCol()] = id;
}

// Vide la case 
void Grille::videCase(const Coord& c) {
    g[c.getLig()][c.getCol()] = VIDE;
}

// Affiche la grille sous forme texte, avec "." pour les cases vides
void Grille::afficher(ostream& os) const{
    for (int i = 0; i < TAILLEGRILLE; ++i) {
        for (int j = 0; j < TAILLEGRILLE; ++j) {
            if (g[i][j] == VIDE) {
                os << ".";
            } else {
                os << "P"; // P pour plein (Temporaire)
            }
        }
        os << "\n";
    }
}

// Renvoie l’identifiant contenu dans la case donnée
int Grille::getCase(const Coord& c) const {
    return g[c.getLig()][c.getCol()];
}

// ------------------ Tests ------------------

TEST_CASE("Nouvelle Grille vide") {
    Grille g;
    for (int i = 0; i < TAILLEGRILLE; ++i) {
        for (int j = 0; j < TAILLEGRILLE; ++j) {
            Coord c(i, j);
            CHECK(g.caseVide(c));
        }
    }
}

TEST_CASE("Placer un animal dans la Grille") {
    Grille g;
    Coord c(5, 10);
    g.setCase(c, 42);  // place un animal avec ID 42
    CHECK_FALSE(g.caseVide(c));
}

TEST_CASE("Vider une case dans la Grille") {
    Grille g;
    Coord c(8, 8);
    g.setCase(c, 77);
    g.videCase(c);
    CHECK(g.caseVide(c));
}

TEST_CASE("Test getCase sur Grille") {
    Grille g;
    Coord c(5, 5);
    CHECK(g.getCase(c) == VIDE); // au départ, elle est vide

    g.setCase(c, 88);
    CHECK(g.getCase(c) == 88);   // maintenant elle contient 88

    g.videCase(c);
    CHECK(g.getCase(c) == VIDE); // retour à vide
}

TEST_CASE("Modification d'une case n'affecte pas les autres") {
    Grille g;
    Coord c1(3, 3);
    Coord c2(4, 4);

    g.setCase(c1, 101);  // Place un animal en (3, 3)
    
    CHECK(g.getCase(c1) == 101);  // Vérifie la case modifiée
    CHECK(g.getCase(c2) == VIDE); // Vérifie que l'autre case reste vide
}
#include "doctest.h"
#include "coord.hpp"
#include <stdexcept>
#include <iostream>
#include <sstream>
#include "ensemble.hpp"

using namespace std;

/**
 * Constructeur de Coord à partir de lignes et colonnes.
 * Vérifie que les coordonnées sont valides dans la grille.
 */
Coord::Coord(int lig, int col){
    if( lig < 0 || lig > TAILLEGRILLE - 1 || col < 0 || col > TAILLEGRILLE){
        throw out_of_range("Coordonnées hors de la grille");
    }
    this->lig = lig;
    this->col = col;
}

// Renvoie la ligne de la coordonnée.

int Coord::getLig() const {
    return lig;
}

// Renvoie la colonne de la coordonnée.

int Coord::getCol() const {
    return col;
}


// Surcharge de l'opérateur << pour afficher une Coord au format (lig, col).
ostream& operator<<(ostream& out, const Coord& c) {
    out << "(" << c.getLig() << ", " << c.getCol() << ")";
    return out;
}

// Surcharge de l'opérateur == pour comparer deux Coord.
bool operator==(const Coord& a, const Coord& b) {
    return a.getLig() == b.getLig() && a.getCol() == b.getCol();
}

// Surcharge de l'opérateur != pour vérifier si deux Coord sont différentes.

bool operator!=(const Coord& a, const Coord& b) {
    return !(a == b);  
}

// Renvoie l'entier associé à une Coord (encodage ligne-colonne).
int Coord::toInt() const {
    return lig * TAILLEGRILLE + col;
}

/**
 * Constructeur de Coord à partir d’un entier entre 0 et TAILLEGRILLE*TAILLEGRILLE - 1.
 * Permet l'encodage inverse de toInt().
 */
Coord::Coord(int value) {
    if (value < 0 || value >= TAILLEGRILLE * TAILLEGRILLE) {
        throw out_of_range("Valeur entière invalide pour Coord");
    }
    lig = value / TAILLEGRILLE;
    col = value % TAILLEGRILLE;
}


// Renvoie un ensemble des voisins valides (8 maximum) de cette Coord.

Ensemble Coord::voisines() const {
    Ensemble e;

    int imin = max(lig - 1, 0);
    int imax = min(lig + 1, TAILLEGRILLE - 1);
    int jmin = max(col - 1, 0);
    int jmax = min(col + 1, TAILLEGRILLE - 1);

    for (int i = imin; i <= imax; ++i) {
        for (int j = jmin; j <= jmax; ++j) {
            if (i != lig || j != col) {  // ne pas ajouter soi-même
                Coord voisin(i, j);
                e.ajoute(voisin.toInt());
            }
        }
    }

    return e;
}

// -------------------- Tests --------------------

TEST_CASE("coord test"){
    Coord c1(10,5);
}

TEST_CASE("coord get test"){
    Coord c2(10,17);
    CHECK(c2.getLig() == 10);
    CHECK(c2.getCol() == 17);
}

TEST_CASE("coord throw test"){
    CHECK_THROWS_AS(Coord(-1,220), out_of_range);
    CHECK_THROWS_AS(Coord(-17,19), out_of_range);
    CHECK_THROWS_AS(Coord(32,111), out_of_range);
}

TEST_CASE("affichage"){
    Coord c3(7,11);
    ostringstream out;
    out << c3;
    CHECK(out.str() == "(7, 11)");
}

TEST_CASE("affichage again"){
    Coord c5(9,20);
    ostringstream out2;
    out2 << c5;
    CHECK(out2.str() == "(9, 20)");
}

TEST_CASE("Comparaison == et !=") {
    Coord a(1, 2);
    Coord b(1, 2);
    Coord c(3, 4);

    CHECK(a == b);     
    CHECK(a != c);     
    CHECK_FALSE(a == c); 
    CHECK_FALSE(a != b); 
}

TEST_CASE("Conversion Coord to int") {
    Coord c(5, 17);
    int val = c.toInt();
    CHECK(val == 5 * TAILLEGRILLE + 17);

    Coord c2(val);
    CHECK(c2.getLig() == 5);
    CHECK(c2.getCol() == 17);
    CHECK(c == c2);

    CHECK_THROWS_AS(Coord(-1), out_of_range);
    CHECK_THROWS_AS(Coord(TAILLEGRILLE * TAILLEGRILLE), out_of_range);
}

TEST_CASE("Coordonnées aux limites") {
    Coord a(0, 0); 
    Coord b(TAILLEGRILLE - 1, TAILLEGRILLE - 1); 

    CHECK(a.toInt() == 0);
    CHECK(b.toInt() == TAILLEGRILLE * TAILLEGRILLE - 1);

    Coord c1(0); 
    Coord c2(TAILLEGRILLE * TAILLEGRILLE - 1);

    CHECK(c1 == a);
    CHECK(c2 == b);
}

TEST_CASE("center test 8 neighbours") {
    Coord c(2, 1);
    Ensemble voisins = c.voisines();
    CHECK(voisins.cardinal() == 8);  // 8 voisins
}

TEST_CASE("C test 5 neighbours") {
    Coord c(3, 0);
    Ensemble voisins = c.voisines();
    CHECK(voisins.cardinal() == 5);  // 5 voisins 
}

TEST_CASE("corner test 3 neighbours") {
    Coord c(0, 0);
    Ensemble voisins = c.voisines();
    CHECK(voisins.cardinal() == 3);  // 3 voisins
}

TEST_CASE("voisines ne contient jamais la coordonnée elle-même") {
    Coord c(10, 10);
    Ensemble voisins = c.voisines();

    CHECK_FALSE(voisins.contient(c.toInt()));
}

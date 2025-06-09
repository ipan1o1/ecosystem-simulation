#include "ensemble.hpp"
#include <stdexcept>
#include <cstdlib> 
#include "doctest.h"
#include "ensemble.hpp"
#include <sstream>
#include "animal.hpp"

using namespace std;

// Constructeur d’un animal : initialise l’identifiant, l’espèce, la coordonnée, la nourriture (si renard) et l’âge
Animal::Animal(int id, Espece espece, const Coord& coord)
    : id(id), espece(espece), coord(coord), nourriture(espece == Renard ? FoodInit : 0), age(0) { 
}

// Renvoie l'identifiant unique de l'animal
int Animal::getId() const {
    return id;
}

// Renvoie la position actuelle de l'animal dans la grille
Coord Animal::getCoord() const {
    return coord;
}

// Renvoie l'espèce de l'animal (Lapin ou Renard)
Espece Animal::getEspece() const {
    return espece;
}

// Renvoie le niveau de nourriture du renard (inutile pour un lapin)
int Animal::getNourriture() const {
    return nourriture;
}

// Retourne l'âge de l’animal
int Animal::getAge() const {
    return age;
}

// Vieillit l'animal d’un tour (incrémentation de son âge)
void Animal::vieillit() {
    age++;
}

// Met à jour la position de l’animal dans la grille
void Animal::setCoord(const Coord& c) {
    coord = c;
}

// Détermine si l’animal doit mourir : par vieillesse ou par faim (si renard)
bool Animal::meurt() const {
    if (age >= maxAge) return true;
    return (espece == Renard && nourriture <= 0);
}

// Détermine si un animal peut se reproduire, selon des conditions différentes pour les lapins et les renards
bool Animal::seReproduit(int nbVoisinsVides) const {
    if (espece == Lapin) {
        // Le lapin se reproduit s'il a assez de place et que la probabilité est satisfaite
        return nbVoisinsVides >= MinFreeBirthLapin && ((rand() % 100) < ProbBirthLapin * 100);
    } else { // Renard
        // Le renard doit avoir assez mangé et réussir un tirage aléatoire
        return nourriture >= FoodReprod && ((rand() % 100) < ProbBirthRenard * 100);
    }    
}

// Le renard mange un lapin : son niveau de nourriture augmente (sans dépasser MaxFood)
void Animal::mange() {
    nourriture = min(nourriture + FoodLapin, MaxFood); 
}

// Le renard jeûne : sa nourriture diminue d'une unité
void Animal::jeune() {
    if (espece == Renard) {
        nourriture--; 
    }
}

// Affiche les informations principales sur l’animal (utile pour le debug)
void Animal::afficher(ostream& os) const {
    os << (espece == Lapin ? "Lapin" : "Renard")
       << " (id=" << id
       << ", coord=" << coord
       << ", nourriture=" << nourriture << ")";
}

// -------------------- Tests --------------------

TEST_CASE("get id"){
    Coord a1(17,10);
    Animal a(17, Renard, a1);
    CHECK(a.getId() == 17);
}

TEST_CASE("get espece"){
    Coord a2(22,38);
    Animal a(34, Lapin, a2);
    CHECK(a.getEspece() == Lapin);
    CHECK_FALSE(a.getEspece() == Renard);
}

TEST_CASE("get coord"){
    Coord a1(19,20);
    Animal a(22, Renard, a1);
    CHECK(a.getCoord() == Coord(19,20));
    CHECK(a.getCoord() == a1);
}

TEST_CASE("set coord"){
    Coord a1(17,10);
    Animal a(17, Renard, a1);
    a.setCoord(Coord(9,1));
    CHECK(a.getCoord() == Coord(9,1));
    Coord b(9,1);
    CHECK(a.getCoord() == b);
}

TEST_CASE("renard at faim = 0"){
    Animal r1(11, Renard, Coord(19,11));
    CHECK(r1.meurt() == false);
}

TEST_CASE("lapin shouldnt die from hunger"){
    Animal l1(19, Lapin, Coord(12,31));
    CHECK_FALSE(l1.meurt());
}

TEST_CASE("hungry renard dead"){
    Animal r2(88, Renard, Coord(37,39));
    for (int i = 0; i < 6; i++) {
        r2.jeune();
    }
    CHECK(r2.meurt());
}

TEST_CASE("lapin never dies from hunger") {
    Animal l2(6, Lapin, Coord(2, 2));
    for (int i = 0; i < 100; i++) {
        l2.jeune();
    }
    CHECK_FALSE(l2.meurt());
}

TEST_CASE("reset faim to 0 for R") {
    Animal r4(7, Renard, Coord(5, 5));
    for (int i = 0; i < 3; i++) {
        r4.jeune();
    }
    CHECK_FALSE(r4.meurt()); 
    r4.mange(); 
    CHECK_FALSE(r4.meurt());
}

TEST_CASE("renard reproduces if well fed and space available") {
    Animal r(25, Renard, Coord(10, 10));

    for (int i = 0; i < 5; ++i) r.mange();

    int voisinsVides = 8;

    bool reproduced = false;

    for (int i = 0; i < 100; ++i) {
        if (r.seReproduit(voisinsVides)) {
            reproduced = true;
            break;
        }
    }

    CHECK(reproduced); // Doit se reproduire au moins une fois sur 100
}

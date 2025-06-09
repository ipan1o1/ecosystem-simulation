#include "ensemble.hpp"
#include <stdexcept>
#include <cstdlib> 
#include "doctest.h"
#include <sstream>

using namespace std;

// Constructeur : initialise un ensemble vide
Ensemble::Ensemble() {
    card = 0;
}

// Renvoie le nombre d'éléments dans l'ensemble
int Ensemble::cardinal() const {
    return card; 
}

// Renvoie true si l'ensemble est vide, sinon false
bool Ensemble::estVide() const {
    return card == 0;
}

// Renvoie true si l'ensemble contient la valeur val
bool Ensemble::contient(int val) const {
    for (int i = 0; i < card; ++i) {
        if (t[i] == val) {
            return true;
        }
    }
    return false;
}

// Ajoute un élément à l'ensemble
void Ensemble::ajoute(int val) {
    if (card >= MAXCARD) {
        throw overflow_error("Ensemble plein");
    }
    t[card] = val; // insère à la fin
    card++;        // incrémente le compteur
}

// Retire un élément de l'ensemble s'il est présent
void Ensemble::retire(int val) {
    for (int i = 0; i < card; ++i) {
        if (t[i] == val) {
            t[i] = t[card - 1]; // remplace par le dernier élément
            card--;             // réduit le nombre total
            return;
        }
    }
    throw std::invalid_argument("Valeur absente de l'ensemble");
}

// Tire un élément au hasard de l'ensemble et le retire
int Ensemble::tire() {
    if (estVide()) {
        throw underflow_error("Ensemble vide");
    }
    int i = rand() % card;
    int val = t[i];

    t[i] = t[card - 1]; // remplace par le dernier
    card--;

    return val;
}

// Affiche les éléments de l'ensemble dans un flux 
void Ensemble::affiche(ostream& os) const {
    os << "{ ";
    for (int i = 0; i < card; i++) {
        os << t[i] << " ";
    }
    os << "}";
}

// Surcharge de l'opérateur << pour l'affichage avec cout
ostream& operator<<(ostream& out, const Ensemble& e) {
    e.affiche(out);
    return out;
}

// Renvoie l'élément à l'indice i
int Ensemble::getElement(int i) const {
    if (i < 0 || i >= card) {
        throw std::out_of_range("Index hors limites dans getElement");
    }
    return t[i];
}

//---------- Tests ---------------

TEST_CASE("emptiness") {
    Ensemble e;
    CHECK(e.estVide());
    CHECK(e.cardinal() == 0);
}

TEST_CASE("Ajout dans un ensemble") {
    Ensemble e;
    e.ajoute(5);
    e.ajoute(10);
    e.ajoute(15);

    CHECK_FALSE(e.estVide());
    CHECK(e.cardinal() == 3);
}

TEST_CASE("random picks") {
    Ensemble e;
    e.ajoute(100);

    int val = e.tire();
    CHECK(val == 100);  

    CHECK(e.estVide()); 
}

TEST_CASE("cant remove from emptiness") {
    Ensemble e;
    CHECK_THROWS_AS(e.tire(), underflow_error);
}

TEST_CASE("cant add anymore") {
    Ensemble e;
    for (int i = 0; i < MAXCARD; ++i) {
        e.ajoute(i);
    }
    CHECK_THROWS_AS(e.ajoute(9999), std::overflow_error);
}

TEST_CASE("add n remove") {
    Ensemble e;
    CHECK(e.estVide());
    e.ajoute(42);
    CHECK(e.cardinal() == 1);
    CHECK_FALSE(e.estVide());

    int val = e.tire();
    CHECK(val == 42);
    CHECK(e.estVide());
}

TEST_CASE("Affichage d'un ensemble avec plusieurs éléments") {
    Ensemble e;
    e.ajoute(1);
    e.ajoute(2);

    ostringstream oss;
    oss << e;

    // Possible outputs: "{ 1 2 }" or "{ 2 1 }" because order could change after tire()
    string output = oss.str();
    CHECK((output == "{ 1 2 }" || output == "{ 2 1 }"));
}

TEST_CASE("getElement from Ensemble") {
    Ensemble e;
    e.ajoute(42);
    e.ajoute(99);

    CHECK((e.getElement(0) == 42 || e.getElement(0) == 99));
    CHECK_THROWS_AS(e.getElement(-1), out_of_range);
    CHECK_THROWS_AS(e.getElement(2), out_of_range); // only 2 elements
}

TEST_CASE("retire un élément de l'ensemble") {
    Ensemble e;
    e.ajoute(10);
    e.ajoute(20);
    e.ajoute(30);

    e.retire(20);

    CHECK(e.cardinal() == 2);
    CHECK_FALSE(e.contient(20)); // vérifie que l'élément a bien été retiré
    CHECK(e.contient(10));
    CHECK(e.contient(30));
}

#include "population.hpp"
#include "ensemble.hpp"
#include <stdexcept>
#include <cstdlib> 
#include "doctest.h"
#include "ensemble.hpp"
#include <sstream>
#include "animal.hpp"

using namespace std;

// Constructeur par défaut : ne fait rien pour l'instant
Population::Population() {}

// Renvoie une référence vers l’animal correspondant à l’ID donné
// Lance une exception si l’ID n’est pas présent ou si le pointeur est nul
Animal& Population::get(int id) const {
    if (!ids.contient(id) || t[id] == nullptr) {
        throw invalid_argument("Animal ID does not exist");
    }
    return *t[id];
}

// Renvoie l’ensemble des identifiants (IDs) actuellement présents dans la population
Ensemble Population::getIds() const {
    return ids;
}

// Réserve un nouvel ID pour un animal à ajouter
// Renvoie le premier ID libre disponible
int Population::reserve() {
    for (int i = 0; i < MAXANIMAUX; i++) {
        if (!ids.contient(i)) {
            ids.ajoute(i);
            return i;
        }
    }
    throw overflow_error("Population full");
}

// Insère un animal dans la population à un ID déjà réservé
void Population::set(const Animal& a) {
    int id = a.getId();
    if (!ids.contient(id)) {
        throw invalid_argument("ID was not reserved before set");
    }
    t[id] = new Animal(a); // Copie dynamique de l’animal
}

// Supprime un animal de la population (libère la mémoire)
void Population::supprime(int id) {
    if (!ids.contient(id)) {
        throw invalid_argument("Cannot delete a non-existing animal");
    }
    ids.retire(id);      // Retire l’ID de l’ensemble
    delete t[id];        // Libère l’objet mémoire
    t[id] = nullptr;     // Nettoie le pointeur
}


// ------ TESTS -----

TEST_CASE("Reserve and set a Lapin in the Population") {
    Population p;
    int id = p.reserve();
    Animal a(id, Lapin, Coord(10, 10));
    p.set(a);

    CHECK(p.getIds().cardinal() == 1);
    CHECK(p.get(id).getId() == id);
    CHECK(p.get(id).getEspece() == Lapin);
    CHECK(p.get(id).getCoord() == Coord(10, 10));
}

TEST_CASE("Reserve and set a Renard in the Population") {
    Population p;
    int id = p.reserve();
    Animal a(id, Renard, Coord(5, 5));
    p.set(a);

    CHECK(p.get(id).getEspece() == Renard);
    CHECK(p.get(id).getCoord() == Coord(5, 5));
}

TEST_CASE("Supprime an Animal from the Population") {
    Population p;
    int id = p.reserve();
    Animal a(id, Lapin, Coord(3, 3));
    p.set(a);

    CHECK(p.getIds().cardinal() == 1);

    p.supprime(id);

    CHECK(p.getIds().cardinal() == 0);
}

TEST_CASE("Accessing non-existent animal throws error") {
    Population p;
    int id = 123; // ID aléatoire non utilisé

    CHECK_THROWS_AS(p.get(id), invalid_argument);
}

TEST_CASE("Setting an Animal without reserving throws") {
    Population p;
    Animal a(42, Lapin, Coord(7,7)); // Animal avec ID non réservé

    CHECK_THROWS_AS(p.set(a), invalid_argument);
}

TEST_CASE("Réutilisation d’un ID après suppression") {
    Population p;
    int id1 = p.reserve();
    Animal a1(id1, Lapin, Coord(2, 2));
    p.set(a1);

    p.supprime(id1); // Suppression du premier animal

    int id2 = p.reserve(); // Réserve à nouveau : doit réutiliser id1
    Animal a2(id2, Renard, Coord(4, 4));
    p.set(a2);

    CHECK(id2 == id1); // Vérifie que l’ID a été réutilisé
    CHECK(p.get(id2).getEspece() == Renard);
}

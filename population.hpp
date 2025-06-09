#ifndef POPULATION_HPP
#define POPULATION_HPP

#include <iostream>
#include <stdexcept>
#include "ensemble.hpp"
#include "coord.hpp"
#include "animal.hpp"

using namespace std;

const int MAXANIMAUX = 5000; // Nombre maximum d’animaux dans la simulation

class Population {
    private:
        Animal* t[MAXANIMAUX]; // Tableau de pointeurs vers les animaux
        Ensemble ids;          // Ensemble des identifiants utilisés

    public:
        // Constructeur par défaut
        Population();

        // Renvoie une référence vers l’animal correspondant à l’ID
        Animal& get(int id) const;

        // Renvoie l’ensemble des IDs actuellement utilisés
        Ensemble getIds() const;

        // Réserve un nouvel ID libre et l’ajoute à l’ensemble
        int reserve();

        // Ajoute un animal dans la population à l’ID donné
        void set(const Animal& a);

        // Supprime un animal donné par son ID (libère la mémoire)
        void supprime(int id);
};

#endif

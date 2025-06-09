#ifndef ANIMAL_HPP
#define ANIMAL_HPP

#include <iostream>
#include <stdexcept>
#include "ensemble.hpp"
#include "coord.hpp"

using namespace std;

// Définition des espèces possibles
enum Espece {Renard, Lapin};

// Âge maximal qu'un animal peut atteindre avant de mourir
const int maxAge = 25;

// Paramètres du comportement des Lapins
const double ProbBirthLapin = 0.30;       // Probabilité de reproduction
const int MinFreeBirthLapin = 4;          // Nombre minimal de cases vides pour se reproduire

// Paramètres du comportement des Renards
const int FoodInit = 5;                   // Niveau de nourriture initial
const int FoodLapin = 5;                  // Nourriture gagnée en mangeant un lapin
const int FoodReprod = 8;                 // Seuil minimal de nourriture pour se reproduire
const int MaxFood = 10;                   // Nourriture maximale
const double ProbBirthRenard = 0.05;      // Probabilité de reproduction

class Animal {
    private:
        int const id;             // Identifiant unique
        Coord coord;              // Position dans la grille
        Espece espece;            // Espèce de l’animal 
        int nourriture;           // Niveau de nourriture (utile que pour les renards)
        int age;                  // Âge de l’animal

    public:
        // Constructeur : initialise un animal avec son id, son espèce et sa position
        Animal(int id, Espece espece, const Coord &coord);

        // Renvoie l’identifiant de l’animal
        int getId() const;

        // Renvoie l’espèce de l’animal
        Espece getEspece() const;

        // Renvoie les coordonnées de l’animal
        Coord getCoord() const;

        // Renvoie le niveau de nourriture
        int getNourriture() const;

        // Renvoie l’âge actuel de l’animal
        int getAge() const;

        // Modifie les coordonnées de l’animal
        void setCoord(const Coord &c);

        // Fait vieillir l’animal 
        void vieillit();

        // Renvoie vrai si l’animal meurt (de faim ou de vieillesse)
        bool meurt() const;

        // Renvoie vrai si l’animal peut se reproduire
        bool seReproduit(int voisinsVides) const;

        // Fait manger l’animal (augmente sa nourriture)
        void mange();

        // Fait jeûner l’animal (diminue sa nourriture)
        void jeune();

        // Affiche les informations de l’animal dans un flux
        void afficher(ostream &os) const;
};

#endif

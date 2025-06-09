#ifndef JEU_HPP
#define JEU_HPP

#include <iostream>
#include <stdexcept>
#include "ensemble.hpp"
#include "coord.hpp"
#include "grille.hpp"
#include "animal.hpp"
#include "population.hpp"

using namespace std;

// Classe principale qui gère la simulation du monde
class Jeu {
    private:
        Grille grille;             // Grille contenant les ID des animaux
        Population population;     // Population d'animaux (objets Animal)
        
    public:
        // Constructeur : initialise la grille avec des lapins et renards selon les probabilités données
        Jeu(double probLapin, double probRenard);

        // Ajoute un animal d'une espèce donnée à une coordonnée précise (si vide)
        void ajouteAnimal(Espece e, const Coord& c);

        // Vérifie que chaque animal est bien dans la grille et que chaque case non vide pointe vers un animal valide
        bool verifieGrille() const;

        // Renvoie une référence constante à la population actuelle
        const Population& getPopulation() const;

        // Renvoie l'ensemble des cases voisines vides autour d'une coordonnée
        Ensemble voisinsVides(const Coord& c) const;

        // Renvoie les cases voisines contenant une espèce donnée
        Ensemble voisinsEspece(const Coord& c, Espece e) const;

        // Déplace un animal vers une case voisine vide choisie aléatoirement
        void deplaceAnimal(int id);

        // Renvoie l'identifiant de l'animal présent à une coordonnée donnée (ou VIDE)
        int getIdAtCoord(const Coord& c) const;

        // Vérifie que la grille et la population sont cohérentes (aucune contradiction)
        void verifieCoherence() const;

        // Affiche les statistiques courantes (nombre de lapins et renards)
        void afficherStats() const;

        // Effectue un tour complet : déplacement, vieillissement, reproduction, mort
        void tour();

        // Affiche la grille sous forme texte dans un flux (ex. cout)
        void afficher(ostream& os) const;

        // Sauvegarde l'état de la grille dans un fichier image PPM (pour affichage graphique)
        void sauvegardePPM(int tour) const;
};

#endif

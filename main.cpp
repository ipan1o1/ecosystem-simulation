#define DOCTEST_CONFIG_IMPLEMENT
#include <iostream>
#include "doctest.h"
#include "coord.hpp"
#include "ensemble.hpp"
#include "jeu.hpp"
#include <unistd.h> // pour sleep()
#include <sstream>
#include <ctime>
using namespace std;

// Nombre total de tours à simuler
const int nbTours = 20;

int main() {
    srand(time(0)); // Initialisation du générateur aléatoire avec l'heure système

    // Création du jeu avec une probabilité de 15% de lapins et 2% de renards
    Jeu jeu(0.15, 0.02);

    // Ouverture d'un fichier pour enregistrer les statistiques à chaque tour
    ofstream stats("stats.csv");
    stats << "Round,Lapins,Renards\n"; // En-tête du fichier CSV

    // Boucle principale de la simulation
    for (int i = 1; i <= nbTours; i++) {
        cout << "\n=== Simulation #" << i << " ===\n";
        
        jeu.afficher(cout);         // Affichage texte de la grille
        jeu.tour();                 // Exécution d'un tour de simulation (mouvements, reproduction, etc)
        jeu.afficherStats();        // Affichage du nombre d'animaux restants
        jeu.sauvegardePPM(i);       // Sauvegarde de l'état de la grille en image PPM (pour la creation du GIF)

        // Comptage du nombre de lapins et de renards
        int lapins = 0;
        int renards = 0;
        Ensemble ids = jeu.getPopulation().getIds();
        for (int j = 0; j < ids.cardinal(); j++) {
            int id = ids.getElement(j);
            if (jeu.getPopulation().get(id).getEspece() == Lapin) lapins++;
            else renards++;
        }

        // Enregistrement des statistiques dans le fichier CSV
        stats << i << "," << lapins << "," << renards << "\n";

        sleep(1); // Pause d'une seconde entre chaque tour
    }

    stats.close(); // Fermeture du fichier
    cout << "\nSimulation terminée. Statistiques enregistrées dans stats.csv\n";
    return 0;
}

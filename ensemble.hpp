#ifndef ENSEMBLE_HPP
#define ENSEMBLE_HPP

#include <iostream>
#include <stdexcept>

using namespace std;

// Nombre maximum d’éléments qu’un ensemble peut contenir
const int MAXCARD = 1600;

class Ensemble {
private:
    int t[MAXCARD]; // Tableau des éléments de l’ensemble
    int card;       // Nombre d’éléments actuellement présents

public:
    // Constructeur : initialise un ensemble vide
    Ensemble();

    // Renvoie le nombre d’éléments dans l’ensemble
    int cardinal() const;

    // Renvoie true si l’ensemble est vide
    bool estVide() const;

    // Renvoie true si la valeur val est dans l’ensemble
    bool contient(int val) const;

    // Ajoute la valeur val à l’ensemble
    void ajoute(int val);

    // Retire la valeur val de l’ensemble
    void retire(int val);

    // Tire et renvoie un élément au hasard de l’ensemble (et le retire de l'ensemble)
    int tire();

    // Affiche les éléments de l’ensemble dans un flux
    void affiche(ostream& os) const;

    // Renvoie l’élément situé à l’indice i
    int getElement(int i) const;
};

// Surcharge de l’opérateur << pour afficher un ensemble
ostream& operator<<(ostream& os, const Ensemble& e);

#endif

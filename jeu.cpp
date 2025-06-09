#include "jeu.hpp"
#include <stdexcept>
#include <cstdlib> 
#include "doctest.h"
#include <sstream>
#include <exception>
#include <iomanip>
#include <fstream>

using namespace std;

// Ajoute un animal sur la grille à une position donnée, avec une espèce (Lapin ou Renard)
void Jeu::ajouteAnimal(Espece e, const Coord& c) {
    if (!grille.caseVide(c)) {
        throw invalid_argument("Impossible de placer un animal dans une case occupée");
    }
    int id = population.reserve();       // Réserve un identifiant unique
    Animal a(id, e, c);                  // Crée l'animal
    population.set(a);                   // Ajoute à la population
    grille.setCase(c, id);               // Met à jour la grille
}

// Constructeur du jeu : initialise la grille avec une probabilité pour les lapins et les renards
Jeu::Jeu(double probLapin, double probRenard) {
    for (int i = 0; i < TAILLEGRILLE; i++) {
        for (int j = 0; j < TAILLEGRILLE; j++) {
            int r = rand() % 100 + 1;
            Coord c(i, j);
            if (r <= probLapin * 100) {
                ajouteAnimal(Lapin, c);
            } else if (r <= (probLapin + probRenard) * 100) {
                ajouteAnimal(Renard, c);
            }
        }
    }
}

// Vérifie que les données entre la grille et la population sont cohérentes
bool Jeu::verifieGrille() const {
    Ensemble ids = population.getIds();
    for (int i = 0; i < TAILLEGRILLE; i++) {
        for (int j = 0; j < TAILLEGRILLE; j++) {
            Coord c(i, j);
            int id = grille.getCase(c);
            if (id != VIDE) {
                if (!ids.contient(id)) return false;
                if (!(population.get(id).getCoord() == c)) return false;
            }
        }
    }
    return true;
}

// Renvoie les coordonnées voisines vides autour d'une case donnée
Ensemble Jeu::voisinsVides(const Coord& c) const {
    Ensemble vides;
    Ensemble voisins = c.voisines();
    for (int i = 0; i < voisins.cardinal(); i++) {
        Coord voisin(voisins.getElement(i));
        if (grille.caseVide(voisin)) {
            vides.ajoute(voisin.toInt());
        }
    }
    return vides;
}

// Renvoie les coordonnées voisines contenant des animaux d'une espèce spécifique
Ensemble Jeu::voisinsEspece(const Coord& c, Espece espece) const {
    Ensemble resultat;
    Ensemble voisins = c.voisines();
    for (int i = 0; i < voisins.cardinal(); ++i) {
        Coord voisin(voisins.getElement(i));
        if (!grille.caseVide(voisin)) {
            int id = grille.getCase(voisin);
            if (population.get(id).getEspece() == espece) {
                resultat.ajoute(voisin.toInt());
            }
        }
    }
    return resultat;
}

// Déplace un animal vers une case vide voisine (aléatoirement)
void Jeu::deplaceAnimal(int id) {
    Animal& a = population.get(id);
    Coord ancienne = a.getCoord();
    Ensemble vides = voisinsVides(ancienne);
    if (vides.estVide()) return;
    Coord nouvelle(vides.tire());
    a.setCoord(nouvelle);
    grille.videCase(ancienne);
    grille.setCase(nouvelle, id);
}

// Renvoie la population actuelle
const Population& Jeu::getPopulation() const {
    return population;
}

// Renvoie l'identifiant de l'animal à une coordonnée donnée
int Jeu::getIdAtCoord(const Coord& c) const {
    return grille.getCase(c);
}

// Affiche la grille textuellement dans un flux donné (L = lapin, R = renard, . = vide)
void Jeu::afficher(ostream& os) const {
    for (int i = 0; i < TAILLEGRILLE; ++i) {
        for (int j = 0; j < TAILLEGRILLE; ++j) {
            int id = grille.getCase(Coord(i, j));
            if (id == VIDE) os << ". ";
            else {
                Espece e = population.get(id).getEspece();
                os << (e == Lapin ? "L " : "R ");
            }
        }
        os << "\n";
    }
}

// Vérifie que chaque animal est bien positionné dans la grille et vice versa
void Jeu::verifieCoherence() const {
    // Récupère les identifiants de tous les animaux actuellement dans la population
    Ensemble ids = population.getIds();

    // Vérifie que chaque animal de la population est bien à sa place dans la grille
    for (int i = 0; i < ids.cardinal(); ++i) {
        int id = ids.getElement(i);
        const Animal& a = population.get(id);
        Coord c = a.getCoord();
        
        // Si l'ID trouvé dans la grille à la position de l'animal est différent de l'ID attendu
        if (grille.getCase(c) != id) {
            cerr << "Incohérence: Animal ID=" << id << " se dit en " << c
                 << " mais la grille contient ID=" << grille.getCase(c) << endl;
            throw runtime_error("Incohérence animal-grille détectée");
        }
    }

    // Vérifie que chaque case de la grille contenant un ID correspond à un animal valide dans la population
    for (int i = 0; i < TAILLEGRILLE; ++i) {
        for (int j = 0; j < TAILLEGRILLE; ++j) {
            Coord c(i, j);
            int id = grille.getCase(c);

            if (id != VIDE) {
                // Si l'ID n'est pas présent dans la population
                if (!ids.contient(id)) {
                    cerr << "Incohérence: La grille contient l'ID " << id
                         << " à la position " << c << " mais cet animal n'existe pas." << endl;
                    throw runtime_error("Incohérence grille-population détectée");
                }

                // Si la coordonnée enregistrée dans l'animal ne correspond pas à la position de la grille
                if (!(population.get(id).getCoord() == c)) {
                    cerr << "Incohérence: L'animal ID=" << id
                         << " est censé être à " << population.get(id).getCoord()
                         << " mais est dans la grille à " << c << endl;
                    throw runtime_error("Incohérence position animal-grille détectée");
                }
            }
        }
    }
}

void Jeu::tour() {
    int mortsAge = 0; // Compteur temporaire pour suivre le nombre de morts dues à l’âge

    // --- Phase 1 : Déplacement des lapins ---
    Ensemble idsLapins = population.getIds(); // Récupère tous les identifiants des animaux existants

    for (int i = 0; i < idsLapins.cardinal(); ++i) {
        int id = idsLapins.getElement(i);
        if (!population.getIds().contient(id)) continue; // L’animal peut avoir été supprimé plus tôt

        Animal& lapin = population.get(id);
        if (lapin.getEspece() != Lapin) continue; // Ne traite que les lapins

        lapin.vieillit(); // Le lapin prend un an

        if (lapin.getAge() > maxAge) {
            // Si le lapin est trop vieux, il meurt
            grille.videCase(lapin.getCoord());
            population.supprime(id);
            mortsAge++;
            continue;
        }

        Coord oldPos = lapin.getCoord();
        Ensemble voisinsLibres = voisinsVides(oldPos);
        int nbVides = voisinsLibres.cardinal();

        // Déplacement : choisit une case vide au hasard
        if (!voisinsLibres.estVide()) {
            int c = voisinsLibres.tire();
            Coord nouvelle(c);
            lapin.setCoord(nouvelle);
            grille.videCase(oldPos);
            grille.setCase(nouvelle, id);
        }

        // Reproduction : si conditions remplies, un nouveau lapin naît à l’ancienne position
        if (nbVides >= MinFreeBirthLapin && (rand() % 100 < ProbBirthLapin * 100)) {
            if (grille.caseVide(oldPos)) {
                ajouteAnimal(Lapin, oldPos);
            }
        }
    }

    // --- Phase 2 : Déplacement des renards ---
    Ensemble idsRenards = population.getIds(); // Récupère les ID restants (après suppression éventuelle de lapins)

    for (int i = 0; i < idsRenards.cardinal(); ++i) {
        int id = idsRenards.getElement(i);
        if (!population.getIds().contient(id)) continue; // Peut avoir été supprimé plus tôt

        Animal& renard = population.get(id);
        if (renard.getEspece() != Renard) continue; // Ne traite que les renards

        Coord oldPos = renard.getCoord();
        renard.vieillit(); // Vieillit d’un an

        if (renard.getAge() > maxAge) {
            // Meurt de vieillesse
            grille.videCase(renard.getCoord());
            population.supprime(id);
            mortsAge++;
            continue;
        }

        renard.jeune(); // Perd un point de nourriture

        if (renard.meurt()) {
            // Meurt de faim
            grille.videCase(oldPos);
            population.supprime(id);
            continue;
        }

        // Recherche un lapin voisin à manger
        Ensemble voisinsLapins = voisinsEspece(oldPos, Lapin);
        Coord newPos = oldPos;

        if (!voisinsLapins.estVide()) {
            // Mange un lapin voisin
            int lapinCoordId = voisinsLapins.tire();
            Coord lapinCoord(lapinCoordId);
            int lapinId = grille.getCase(lapinCoord);
            population.supprime(lapinId);      // Retire le lapin
            grille.videCase(lapinCoord);
            renard.mange();                    // Gagne de la nourriture
            newPos = lapinCoord;               // Se déplace à l’ancienne position du lapin
        } else {
            // Sinon, cherche une case vide pour se déplacer
            Ensemble voisinsLibres = voisinsVides(oldPos);
            if (!voisinsLibres.estVide()) {
                int coordId = voisinsLibres.tire();
                newPos = Coord(coordId);
            }
        }

        // Applique le déplacement
        if (!(newPos == oldPos)) {
            renard.setCoord(newPos);
            grille.videCase(oldPos);
            grille.setCase(newPos, id);
        }

        // Reproduction si bien nourri et case disponible
        if (renard.getNourriture() >= FoodReprod && (rand() % 100 < ProbBirthRenard * 100)) {
            if (grille.caseVide(oldPos)) {
                ajouteAnimal(Renard, oldPos);
            }
        }
    }

    // Vérifie la cohérence grille <-> population à la fin du tour
    verifieCoherence();
}


// Affiche les statistiques actuelles : nombre de lapins et de renards
void Jeu::afficherStats() const {
    int nLapins = 0;
    int nRenards = 0;
    Ensemble ids = population.getIds();
    for (int i = 0; i < ids.cardinal(); ++i) {
        int id = ids.getElement(i);
        Espece e = population.get(id).getEspece();
        if (e == Lapin) nLapins++;
        else nRenards++;
    }
    cout << "Lapins: " << nLapins << " | Renards: " << nRenards << endl;
}

// Sauvegarde une image de la grille au format PPM pour le GIF
void Jeu::sauvegardePPM(int tour) const {
    ostringstream filename;
    filename << "img" << setfill('0') << setw(3) << tour << ".ppm";
    ofstream out(filename.str());
    out << "P3\n" << TAILLEGRILLE << " " << TAILLEGRILLE << "\n255\n";
    for (int i = 0; i < TAILLEGRILLE; ++i) {
        for (int j = 0; j < TAILLEGRILLE; ++j) {
            Coord c(i, j);
            int id = grille.getCase(c);
            if (id == VIDE) {
                out << "255 255 255 "; // blanc
            } else {
                Espece e = population.get(id).getEspece();
                if (e == Lapin) out << "0 0 255 ";  // bleu
                else out << "255 0 0 ";             // rouge
            }
        }
        out << "\n";
    }
    out.close();
}

//-------------Tests -------------

TEST_CASE("Grille correspond à la population") {
    Jeu j(0.1, 0.1);
    CHECK(j.verifieGrille());
}

TEST_CASE("voisinsVides renvoie uniquement les cases voisines vides") {
    Jeu j(0.0, 0.0); // grille vide au départ
    Coord c(5, 5);
    CHECK(j.voisinsVides(c).cardinal() == c.voisines().cardinal()); // toutes les 8 cases doivent être vides
}

TEST_CASE("voisinsEspece renvoie correctement les espèces") {
    Jeu j(0.0, 0.0); // grille vide au départ
    Coord centre(10, 10);
    Coord lapinPos(10, 11);
    Coord renardPos(9, 9);

    j.ajouteAnimal(Lapin, lapinPos);
    j.ajouteAnimal(Renard, renardPos);

    Ensemble lapins = j.voisinsEspece(centre, Lapin);
    Ensemble renards = j.voisinsEspece(centre, Renard);

    CHECK(lapins.cardinal() == 1);
    CHECK(renards.cardinal() == 1);
}

TEST_CASE("deplaceAnimal déplace l'animal vers une case voisine libre") {
    Jeu j(0.0, 0.0); // grille vide

    Coord depart(10, 10);
    j.ajouteAnimal(Lapin, depart);

    // Récupère l'identifiant du seul animal dans la population
    Ensemble ids = j.getPopulation().getIds();
    int id = ids.getElement(0); // on a ajouté un seul animal

    // Appelle la fonction de déplacement
    j.deplaceAnimal(id);

    // Vérifie la nouvelle position de l’animal
    Coord nouvellePos = j.getPopulation().get(id).getCoord();

    // Vérifie que la nouvelle position est une voisine de l’ancienne
    Ensemble voisins = depart.voisines();
    CHECK(voisins.contient(nouvellePos.toInt()));

    // Vérifie qu'il s'est effectivement déplacé
    CHECK(nouvellePos != depart);
}

TEST_CASE("Lapin se déplace pendant un tour") {
    Jeu j(0.0, 0.0);  // Grille vide
    Coord c(5, 5);
    j.ajouteAnimal(Lapin, c);

    int id = j.getPopulation().getIds().getElement(0);
    Coord avant = j.getPopulation().get(id).getCoord();

    j.tour();  // Exécute un tour

    Coord apres = j.getPopulation().get(id).getCoord();
    CHECK(avant != apres); // Le lapin doit s'être déplacé
    CHECK(avant.voisines().contient(apres.toInt())); // La nouvelle position est voisine de l'ancienne
}

TEST_CASE("Renard meurt de faim") {
    Jeu j(0.0, 0.0);
    Coord c(5, 5);
    j.ajouteAnimal(Renard, c);

    int id = j.getIdAtCoord(c); // identifiant précis du renard

    // Simule 6 tours sans nourriture
    for (int i = 0; i < 6; ++i) {
        j.tour();
    }

    CHECK_FALSE(j.getPopulation().getIds().contient(id)); // Le renard devrait être mort
}

TEST_CASE("Lapin peut se reproduire si conditions remplies") {
    Animal l(1, Lapin, Coord(3, 3));
    bool peutNaître = false;

    // On force la reproduction à réussir en répétant plusieurs essais
    for (int i = 0; i < 100; ++i) {
        if (l.seReproduit(5)) {
            peutNaître = true;
            break;
        }
    }

    CHECK(peutNaître);  // Le lapin doit réussir à se reproduire au moins une fois sur 100 essais
}


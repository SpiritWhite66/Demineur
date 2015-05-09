#ifndef DEF_DEMINEUR
#define DEF_DEMINEUR

#include <iostream>

class Demineur
{
public:

    Demineur(); //constructeur
    ~Demineur();
    void Start(); //permet de gérer le démarrage et le redemarrage
    void legend();
    void InitGrille();
    void InitBombe();
    void InitBonus();
    void InitMalus();
    void evenement_bonus(); 
    void evenement_malus(); 
    int  Aleatoire(int max);
    bool LostGame();
    bool WinGame();
    void Affiche();
    void SelectCase();

private:
    int * test;
    int taille; // taille de la grille
    int revele; // nombre de case révélé
    int nbr_bombe;
    bool **etat; //contient l'état de la case, true vide, false caché 
    int **event; //contient le contenu de la case
    bool gamer;
    int case_x; 
    int case_y; 
    bool state; //dit si la case est déjà révéler 
    int end; //0 => dans une parti, 1=> on continu, 2=> on termine
    int random_event;
    int random_save;
};

#endif 

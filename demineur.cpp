#include "demineur.hpp"
#include <iostream>
#include <random>
using namespace std;
 

/***********************************************************************
/*************************** Documentation : ***************************
/***********************************************************************
/   Utilisation de la programmation orienté objet 
/   Avantage : Code plus clair, modulaire, modification plus simple.
/
/   Deux tableaux bidimensionnel, un de type boolèen, l'autre de type int. 
/   - Le premier tableau, celui de type boolèen renseigne sur l'état de la case, caché(false) ou révélé(true)
/   - Le deuxième tableau renseigne sur le contenu de la case, vide(0), bombe(1), malus(3) ou bonus(2). 
/   Avantage : facilité d'utilisation et permet de découper deux types d'informations (le contenu et l'état).
/ 
/   Le code est fait de sorte qu'on navigue dans les y puis dans les x (exemple : etat[y][x])
/   Avantage : Facilité d'affichage
/
/   Utilisation de la lib random du c++11
/   Avantage permet de générer plusieurs nombre aléatoire dans la même seconde (plus compliqué avec la lib C) 
/
/   Les deux joueurs sont représenté par un boolèen : true = joueur 1, false = joueur 2
/   Avantage: limite les erreurs pouvant être du au incrémentation (et donc d'inventé un 3eme joueur)
/
/   ___________________________
/   |Legende :                |
/   |-------------------------|
/   |Caché : false | signe : .|
/   |Vide  : 0     | signe : X|
/   |Bombe : 1     | signe : O|
/   |Bonus : 2     | signe : B|
/   |Malus : 2     | signe : M|
    |_________________________|

***********************************************************************/

Demineur::Demineur()

{
      
}
Demineur::~Demineur() // On évite les fuites de mémoire ... c'est mieux :)
{
    for (int i = 0; i < taille; i++)
    {
        delete[] etat[i]; //on détruit chaque case une par une
        delete[] event[i];
    }
    delete[] etat; // puis on détruit le contenant
    delete[] event;
}

void Demineur::Start()
{
    gamer = true; 
    revele = 0;

    do // on demande la taille de la grille
    {
        cout<<"Taille du démineur : [entre 4*4 et 10*10]"<<endl;
        cin >> taille;
    }while(taille<4 or taille>10);

    // on fait un tableau bidimensionnel... en mettant allouant dynamiquement, c'est lourd comme méthode mais je voyais pas d'autre méthodes pour l'adapter à la POO...
    etat = new bool* [taille-1]; 
    event = new int* [taille-1];

   for (int i = 0; i < taille; i++)
   {
        etat[i] = new bool[taille];
        event[i] = new int[taille];
   }

   nbr_bombe = taille - 2;
}
void Demineur::InitGrille() //On cache toute nos case et on met le contenu en "vide"
{
    for(int i =0; i<taille; i++)
    {D
        for(int j =0; j<taille; j++)
        {
            etat[i][j] = false;
            event[i][j] = 0;
        }
    } 
}

void Demineur::InitBombe() // On met en place nos bombes de façons aléatoire en générant un x et un y
{
     int x,y;
    for(int i = 0; i<(taille-2); i++)
    {
        do
        {
            x = Aleatoire(taille);
            y = Aleatoire(taille);
        }while(event[y][x] != 0);
        event[y][x] = 1;
    }
   
}

void Demineur::InitBonus() // On met en place nos bonus de façons aléatoire en générant un x et un y 
{
    int x,y;
    for(int i = 0; i<(taille-3); i++)
    {
       do
        {
            x = Aleatoire(taille);
            y = Aleatoire(taille);
        }while(event[y][x] != 0);
        event[y][x] = 2;
    }
}
void Demineur::InitMalus() // On met en place nos malus de façons aléatoire en générant un x et un y
{
    int x,y;
    for(int i = 0; i<(taille-3); i++)
    {
        do
        {
            x = Aleatoire(taille);
            y = Aleatoire(taille);
        }while(event[y][x] != 0);
        event[y][x] = 3;
    }
}

void Demineur::evenement_malus() 
{
    int random;
    int x = 0; 
    int y = 0; 
    int k = 0;
    random_event = Aleatoire(2); // on met le max qu'on veut (en partant de 1, la méthode Aleatoire mettra l'intervalle entre 0 et 1)
        if (random_event == 0)
        {
            cout<<"Vous etes tombé sur le malus qui Cache la moité des cases révélés et qui bouge la moitié des bombes :'( désolé"<<endl;
            int n = revele/2; // arrondi tout seul en dessous

            /***************************************/
            /***** Cache la moitié des révélés *****/
            /***************************************/
            for(int q = 0; q<n; q++) 
                     {
                    x=0;
                    y=0;
                    k=0;

                    do // permet d'obliger à avoir la chaque sélection différente
                    {
                        random_save=random;
                        random = Aleatoire(revele)+1; //pour avoir un intervalle [1; revele]
                    }while(random==random_save);

                    for(int i = 0; i<taille and k!=random; i++) // on va chercher la case numéro "random"
                    {
                        for(int j = 0; j<taille and k!=random; j++)
                        {
                            y = i;
                            x = j;

                            if(etat[i][j] == true) //à chaque fois qu'on trouve une case révélé on incremente notre "curseur" k
                            {
                               k++;
                            }
                        }
                    }      

                    revele--; 
                    etat[y][x]=false;//on modifie l'état de la cas en case "vide"
                }

            /****************************************/
            /***** Déplace la moitié des bombes *****/
            /****************************************/
             for(int i = 0; i<(nbr_bombe)/2; i++)
            {
                x=0;
                y=0;
                k=0;

                random = Aleatoire(nbr_bombe)+1; //pour avoir un intervalle [1; nbr_bombe]

                //supprime la bombe
                for(int i = 0; i<taille and k!=random; i++) // on va chercher la mine numéro "random"
                {
                    for(int j = 0; j<taille and k!=random; j++)
                    {
                        y = i;
                        x = j;
                        if(event[i][j] == 1) // on compte les bombes
                        {
                           k++;
                        }
                    }
                }

                event[y][x]=0;
                etat[y][x]=false;

                // Rajoute la bombe supprimé dans un case aléatoire
                random = Aleatoire((taille*taille-revele))+1; //pour avoir un intervalle [1; taille*taille-revele)]
                for(int i = 0; i<taille and k!=random; i++) // on va chercher la mine numéro "random"
                {
                    for(int j = 0; j<taille and k!=random; j++)
                    {
                        y = i;
                        x = j;

                    if(etat[i][j] == false or event[i][j]==0) //à chaque fois qu'on trouve une case caché et vide on incremente notre "curseur" k
                    {
                        k++;
                    }
                }
                }
            
                event[y][x]=1;
                etat[y][x]=false;
            }


        }  
        else if(random_event==1)
        {
             cout<<"Vous etes tombé sur le malus qui ajoute une bombe :'( désolé"<<endl;
            x=0;
            y=0;
            k=0;
            random = Aleatoire((taille*taille-revele))+1; //pour avoir un intervalle [1; taille*taille-revele)] 

           for(int i = 0; i<taille and k!=random; i++) // on va chercher la case numéro "random"
            {
                for(int j = 0; j<taille and k!=random; j++)
                {
                        y = i;
                        x = j;

                    if(etat[i][j] == false or event[i][j]==0) 
                    {
                        k++;
                    }
                }
            }
            
            event[y][x]=1;
            etat[y][x]=false;
            cout<<endl<<x<<"|"<<y<<"|"<<random<<endl;
            nbr_bombe++; //on rajoute une bombe au compteur de bombe
        }
}

void Demineur::evenement_bonus() 
{
    int random;
    random_event = Aleatoire(2); // on met le max qu'on veut (en partant de 1, la méthode Aleatoire mettra l'intervalle entre 0 et 1)
        if(random_event == 0)
        {
            cout<<"Vous etes tombé sur le bonus qui supprime une bombe. Merci qui ? :D "<<endl;
            int x,y,k = (0,0,0);
            random = Aleatoire(nbr_bombe)+1; // on choisit aléatoirement la mine à retirer
            for(int i = 0; i<taille and k!=random; i++) // on va chercher la mine numéro "random"
            {
                for(int j = 0; j<taille and k!=random; j++)
                {
                        y = i;
                        x = j;

                    if(event[i][j] == 1) //à chaque fois qu'on trouve une case avec une bombe on incremente notre "curseur" k
                    {
                        k++;
                    }
                }
            }
            event[y][x]=0; //on modifie l'état de la cas en case "vide"
            nbr_bombe--; //on enleve une bombe au compteur
        }    
        else if(random_event==1)
        {
            cout<<"Vous etes tombé sur le bonus qui révèle une case. Merci qui ? :D "<<endl;
            int x,y,k = (0,0,0);
            random = Aleatoire((taille*taille-revele))+1; 
            for(int i = 0; i<taille and k!=random; i++) // on va chercher la casz caché numéro "random"
            {
                for(int j = 0; j<taille and k!=random; j++)
                {
                        y= i;
                        x = j;

                    if(etat[i][j] == false)
                    {
                        k++;
                    }
                }
            }
            etat[y][x]=true;
            revele++;
        }
}

void Demineur::Affiche() 
{
    //On trace notre grille 
    cout<<"  ";
    for(int i =1; i<taille+1; i++) // on place les numéro de l'axe x
    {
        cout<<i; 
    }
    cout<<endl;
    cout<<"  ";
    for(int i =1; i<taille+1; i++) // on trace un trait ... 
    {
        cout<<"-"; 
    }
    cout<<endl;

    for(int i =0; i<taille; i++) // on met nos case
    {
        cout<< i+1 <<"|";
        for(int j =0; j<taille; j++)
        {
            if(etat[i][j]==false)
            {
                cout<<".";
            }
            else if(event[i][j]==0)
            {
                cout<<"x";
            }
            else if(event[i][j]==1)
            {
                cout<<"O";
            }
            else if(event[i][j]==2)
            {
                cout<<"B";
            }
            else if(event[i][j]==3)
            {
                cout<<"M";
            }
        }
        cout<<endl;
    } 

} 

void Demineur::SelectCase() 
{
    if(gamer == true) //On Dit à qui c'est de jouer
    {
        cout<< "Au joueur 1 de jouer"<<endl;
    }
    else
    {
        cout<< "Au joueur 2 de jouer"<<endl;
    }

    state = true; // la variable state permet d'afficher le message d'erreur si on veux afficher une case déjà révélé ... je suis trop gentil...
    do 
    {
        if(state==false)
        {
            cout<<":o Mais elle est déjà révélé... mais comme je suis gentil, je te laisse rejouer..."<<endl;
        }

        do
        {
            cout<< "Rentrer la coordonée x de la case à sélectionner : [entre 1 et "<< taille<<" ]"; 
            cin >>case_x;
        }while(case_x<1 or case_x>taille );
        do
        {
            cout<< "Rentrer la coordonée y de la case à sélectionner : [entre 1 et "<< taille<<" ]"; 
            cin >>case_y;
        }while(case_y<1 or case_y>taille);
        state=false; 

    }while(etat[case_y-1][case_x-1] == true);

    etat[case_y-1][case_x-1] = true;//Et  pouf voici une magnifique case découverte

    if(event[case_y-1][case_x-1] == 2) // on vérifie quand même si y a pas un bonus ou un malus dedans... (LostGame() s'occupe de vérifier si y a pas une bombe ... )
    {
        evenement_bonus();
    }
    else if(event[case_y-1][case_x-1] == 3)
    {
        evenement_malus();
    }
    revele++;
    gamer = !gamer;
}

bool Demineur::LostGame()
{
    if(event[case_y-1][case_x-1] == 1) // on verifie si dans la case de coord(x,y) s'il y a une bombe
    {

        cout<< "BOOOOOOOOOOM ! Tu es faible petit ... tu viens de perdre..."<<endl<< "Le joueur ";
        if (gamer) // on choisit le texte en fonction du joueur (rappel true = joueur 1 ,false = joueur 2)
        {
            cout<<"1 à gagné !";
        }
        else
        {
            cout<<"2 à gagné !";
        }
        cout<<endl;
        return false; // false = il y a une bombe
    }
    else
    {
        return true; // true = il n'y a pas de bombe
    }
}

bool Demineur::WinGame() //Dit 
{
    cout<< "victoire"<< revele+nbr_bombe<< "/" <<taille*taille<<endl;
    if(event[case_y-1][case_x-1] == 0) // on verifie si dans la case de coord(x,y) on vérifie bien que la dernière case est bien une "vide" (les autres peuvent influer sur le nombre de case à découvrir)
    {
        if((revele+nbr_bombe)==(taille*taille))
        {
            cout<< "Tu viens de gagner !"<<endl<< "Le joueur ";
            if (!gamer) // on choisit le texte en fonction du joueur (rappel true = joueur 1 ,false = joueur 2), vu qu'on est passé au 'joueur inverse' juste avant on re-inverse pour avoir le gagnant
            {
            cout<<"1 à gagné !";
            }
            else
            {
            cout<<"2 à gagné !";
            }
            return true; // Tu as gagné ! 
        }
        
    }
    else
    {   
        return false; // true = il n'y a pas de bombe
    }
}


int Demineur::Aleatoire(int max) 
{
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_real_distribution<double> dist(0, max); // on détermine un nombre aléatoire entre 0 et max
    int a = dist(mt);
    return a;
}

void Demineur::legend() //Affiche la légende en début de jeu
{
cout<<"  ___________________________"<<endl<<
"  |Legende :                |"<<endl<<
"  |-------------------------|"<<endl<<
"  |Caché : false | signe : .|"<<endl<<
"  |Vide  : 0     | signe : X|"<<endl<<
"  |Bombe : 1     | signe : O|"<<endl<<
"  |Bonus : 2     | signe : B|"<<endl<<
"  |Malus : 2     | signe : M|"<<endl<<
"  |_________________________|"<<endl<<endl<<endl;
}
#include <iostream>
#include "demineur.hpp"

using namespace std; 

int main(void)
{
	char restart; 
	Demineur demin;
	do
	{	
		demin.Start();
		demin.legend();
		demin.InitGrille();
		demin.InitBombe();
		demin.InitBonus();
		demin.InitMalus();
		demin.Affiche();

		bool save_lostgame = true;
		bool save_wingame = false; 
		while(save_lostgame and !save_wingame)
		{
			demin.SelectCase();
			demin.Affiche();
			save_lostgame = demin.LostGame();
			save_wingame = demin.WinGame();
		}
		cout<<endl<<endl<<"La partie est terminé, voulez vous recommencez ? (O = oui, N = non) ";
		cin>>restart;
	}while(restart == 'O');
	return 0;
	
}
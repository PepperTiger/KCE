#pragma once
#include "Game.h"

class Game;

// Classe abstraite, servant de base pour toutes int�raction des joueurs avec une partie.
class Player
{
public:
	// Fonction retournant le mouvement � jouer apr�s d�cision.
	virtual Move play(Game& game, Color color);
	// Fonction affichant le processus de d�cision.
	virtual void printState();
	// Fonction utlis� pour les mises-�-jour apr�s fin de partie.
	virtual void afterEnd(Game& game, Color color);
	// Fonction utilis� pour les mises-�-jour apr�s mouvement du joueur oppos�.
	virtual void afterOpponentMove(Game& game, Color color, Move move);
};




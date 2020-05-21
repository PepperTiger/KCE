#pragma once
#include "Game.h"

class Game;

// Classe abstraite, servant de base pour toutes intéraction des joueurs avec une partie.
class Player
{
public:
	// Fonction retournant le mouvement à jouer après décision.
	virtual Move play(Game& game, Color color);
	// Fonction affichant le processus de décision.
	virtual void printState();
	// Fonction utlisé pour les mises-à-jour après fin de partie.
	virtual void afterEnd(Game& game, Color color);
	// Fonction utilisé pour les mises-à-jour après mouvement du joueur opposé.
	virtual void afterOpponentMove(Game& game, Color color, Move move);
};




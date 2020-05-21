#pragma once
#include "Player.h"

// Structure permettant le QLearning.
struct QLearning : public Player {
	// QTable, contenant la r�compense escompt�e selon un �tat et une action?
	std::unordered_map<Board, std::unordered_map<Move, double>> qTable;
	// Probabilit� de choisir une action al�atoire.
	double explorationProbability = 0;
	// Taux d'apprentissage.
	double learningRate = 0;
	// Facteur d'actualisation.
	double discountFactor = 0;

	// Variables utilis�es pour �viter la recherche r�pet� dans la QTable.
	Board currentQState;
	Move chosenQAction;
	double currentQValue = 0;
	Board nextQState;

	// Variables utilis�es pour des questions d'affichages.
	double reward = 0;
	double oldQualityValue = 0;
	double optimalNextQualityValue = 0;
	double newQualityValue = 0;
	bool knownState;
	bool knownNextState;
	bool bestActionChosen;
	double bestActionQValue = 0;

	QLearning();
	QLearning(double explorationProbability, double learningRate, double discountFactor);
	// Fonction retournant l'action � jouer apr�s d�cision.
	Move run(Game& game, Color color);
	// Fonction r�cuperant l'�tat et mettant � jour la QTable s'il le faut.
	void getEnvironement(Game& game, Color color);
	// Fonction choissisant � la meilleure action avec une certaine probabilit� et la retourne.
	Move chooseAction();
	// Retourne la meileure action selon un �tat donn�.
	Move bestAction();
	// Met � jour la QTable selon un �tat et une action donn�.
	void updateQTable(Game& game, Color color);
	// Retourne la r�compense selon l'issue de la partie.
	double getReward(Game& game, Color color);
	// Retourne la valeur optimale de l'�tat prochain.
	double optimalNextValue();

	// Fonctions h�rit�es de Player
	Move play(Game& game, Color color) override;
	void afterOpponentMove(Game& game, Color color, Move move) override;
	void afterEnd(Game& game, Color color) override;
	void printState() override;
};

void RunQLearning(double learningRate, double discountFactor, double explorationProbability,
	int minGames, int maxGames);


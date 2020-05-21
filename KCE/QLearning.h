#pragma once
#include "Player.h"

// Structure permettant le QLearning.
struct QLearning : public Player {
	// QTable, contenant la récompense escomptée selon un état et une action?
	std::unordered_map<Board, std::unordered_map<Move, double>> qTable;
	// Probabilité de choisir une action aléatoire.
	double explorationProbability = 0;
	// Taux d'apprentissage.
	double learningRate = 0;
	// Facteur d'actualisation.
	double discountFactor = 0;

	// Variables utilisées pour éviter la recherche répeté dans la QTable.
	Board currentQState;
	Move chosenQAction;
	double currentQValue = 0;
	Board nextQState;

	// Variables utilisées pour des questions d'affichages.
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
	// Fonction retournant l'action à jouer après décision.
	Move run(Game& game, Color color);
	// Fonction récuperant l'état et mettant à jour la QTable s'il le faut.
	void getEnvironement(Game& game, Color color);
	// Fonction choissisant à la meilleure action avec une certaine probabilité et la retourne.
	Move chooseAction();
	// Retourne la meileure action selon un état donné.
	Move bestAction();
	// Met à jour la QTable selon un état et une action donné.
	void updateQTable(Game& game, Color color);
	// Retourne la récompense selon l'issue de la partie.
	double getReward(Game& game, Color color);
	// Retourne la valeur optimale de l'état prochain.
	double optimalNextValue();

	// Fonctions héritées de Player
	Move play(Game& game, Color color) override;
	void afterOpponentMove(Game& game, Color color, Move move) override;
	void afterEnd(Game& game, Color color) override;
	void printState() override;
};

void RunQLearning(double learningRate, double discountFactor, double explorationProbability,
	int minGames, int maxGames);


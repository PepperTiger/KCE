#pragma once
#include "Game.h"
#include "Negamax.h"
#include "XCS.h"
#include "QLearning.h"
#include <string>
#include <fstream>
#include <iostream>
#include <filesystem>
#include <regex>

// Fonction d'interface principale.
void runKCE();

// Fonction d'interface de modification des r�gles de mouvements et de r�p�tition.
void modifyCounter(unsigned int& moveRuleCounter, unsigned int& foldRepetitionCounter);

// Fonction de pr�paration de la proc�dure d'apprentissage.
void prepareLearningProcedure(unsigned int moveRuleCounter, unsigned int foldRepetitionCounter);

// Fonction de pr�paration de la proc�dure d'apprentissage de Q-Learning.
void prepareQLearningProcedure(unsigned int moveRuleCounter, unsigned int foldRepetitionCounter);
// Fonction d'ex�cution de la proc�dure d'apprentissage de Q-Learning.
void runQLearningProcedure(std::string profileName,
	double explorationProbability, double learningRate, double discountFactor,
	unsigned int minGames, bool display,
	unsigned int moveRuleCounter, unsigned int foldRepetitionCounter);

// Fonction de sauvegarde d'une instance de Q-Learning.
void saveQLearning(QLearning& qLearning, std::string profileName);

// Fonction de chargement d'une instance de Q-Learning.
QLearning loadQLearning(std::string profileName);

// Fonction de pr�paration de la proc�dure d'apprentissage de XCS.
void prepareXCSLearningProcedure(unsigned int moveRuleCounter, unsigned int foldRepetitionCounter);
// Fonction d'ex�cution de la proc�dure d'apprentissage de XCS.
void runXCSLearningProcedure(std::string profileName,
	int populationSize, double learningRate, double alpha,
	double errorThreshold, double power, double discountFactor,
	int geneticAlgorithmThreshold, double crossoverProbability,
	double mutationProbability, int experiencDeletionThreshold,
	double fitnessDeletionThreshold, int experienceSubsumptionThreshold,
	double jokerProbability, double initialPrediction,
	double initialPredictionError, double initialFitness,
	double explorationProbability, bool doGeneticAlgorithmSubsumption,
	bool doActionSetSubsumption,
	unsigned int minGames, bool display,
	unsigned int moveRuleCounter, unsigned int foldRepetitionCounter);
// Fonction de sauvegarde d'une instance de XCS.
void saveXCS(XCS& xcs, std::string profileName);
// Fonction de chargement d'une instance de XCS.
XCS loadXCS(std::string filePath, bool learning);

// Condition d'arr�t des proc�dure d'apprentissage.
bool stopCondition();

// Fonction de pr�paration de la proc�dure de test.
void prepareTestProcedure(unsigned int moveRuleCounter, unsigned int foldRepetitionCounter);
// Fonction d'ex�cution de la proc�dure de test.
std::array<int, 3> runTestProcedure(unsigned int moveRuleCounter, unsigned int foldRepetitionCounter,
	std::shared_ptr<Player>& white, std::shared_ptr<Player>& black,
	unsigned int numberGames, bool display);

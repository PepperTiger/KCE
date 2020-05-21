#pragma once
#include "Player.h"

// Structure définissant le contenu d'un classeur dans XCS.
class Classifier {
public:
	// Condition du classeur, extraite d'un état.
	std::array<int, 64> condition;
	// Action préconisé par le classeur.
	Move action;
	// Prédiction du classeur.
	double prediction = 0;
	// Erreur de prédiction du classeur.
	double predictionError = 0;
	// Aptitude du classeur.
	double fitness = 0;
	// Expérience du classeur.
	double experience = 0;
	// Pas de temps depuis la dernière application d'un algorithme génétique sur le classeur.
	int timeStamp = 0;
	// Taille moyenne des ensembles d'actions dont ce classeur a fait partie.
	double actionSetSize = 0;
	// Numérosité du classeur.
	int numerosity = 0;

	Classifier();
	Classifier(const Classifier& cl);
	//~Classifier();
	bool operator==(const Classifier& cl) const;
};

namespace std {
	template<> struct hash<Classifier> {
		std::size_t operator()(Classifier const& cl) const;
	};
	template<> struct equal_to<Classifier> {
		bool operator()(const Classifier& cl1, const Classifier& cl2) const;
	};
}

// Structure définissant l'environnement utilisé par XCS pour récupérer la situation.
struct Environement {
	std::array<int, 64> board;
	Environement();
	//~Environement();
};

// Structure définissant XCS et son fonctionnement.
class XCS : public Player {
public:
	// Enumération définissant l'identifiant de la valeur joker, ou valeur de généralisation.
	enum Joker {
		XCS_Joker = 20
	};

	// Ensemble de population.
	std::vector<Classifier*> population;
	// Ensemble d'action précédent.
	std::vector<Classifier*> previousActionSet;
	// Récompense précédente.
	double previousReward = 0;
	// Environnement précédent.
	Environement previousEnvironement;
	// Actions précédentes.
	std::vector<Move> previousActions;
	// Pas de temps actuel.
	int currentTime = 0;

	// Taille maximale de la population.
	int populationSize;
	// Taux d'apprentissage.
	double learningRate;
	// Taux d'apprentissage de l'aptitude.
	double alpha;
	// Seuil en dessous lequel une erreur de prédiction est considéré comme nul.
	double errorThreshold;
	// Paramètre utilisé pour le calcul de l'aptitude.
	double power;
	// Facteur d'actualisation.
	double discountFactor;
	// Seuil d'activation du l'algorithme génétique.
	int geneticAlgorithmThreshold;
	// Probabiltié de croisement.
	double crossoverProbability;
	// Probabilité de mutation.
	double mutationProbability;
	// Seuil de prise en compte de l'expérience dans le processus de suppression.
	int experienceDeletionThreshold;
	// Seuil de prise en compte de l'aptitude dans le processsu de suppression.
	double fitnessDeletionThreshold;
	// Seuil d'expérience au delà duquel un classeur est éligile pour subsumer d'autres classeurs.
	int experienceSubsumptionThreshold;
	// Probabilité d'appliquer une valeur de généralisation au lieu d'une valeur exacte lors de la constitution d'une condition.
	double jokerProbability;
	// Prédiction initiale.
	double initialPrediction;
	// Erreur de prédiction initiale.
	double initialPredictionError;
	// Aptitude initiale.
	double initialFitness;
	// Probabilité d'utiliser une action aléatoire. 
	double explorationProbability;
	// Condition d'utilisation de la subsumption dans l'algorithme génétique.
	bool doGeneticAlgorithmSubsumption;
	// Condition d'utiisation de la subsumption dans l'ensemble d'action.
	bool doActionSetSubsumption;
	// Condition d'apprentissage.
	bool learning = true;

	XCS();
	XCS(int populationSize, double learningRate, double alpha,
		double errorThreshold, double power, double discountFactor,
		int geneticAlgorithmThreshold, double crossoverProbability,
		double mutationProbability, int experiencDeletionThreshold,
		double fitnessDeletionThreshold, int experienceSubsumptionThreshold,
		double jokerProbability, double initialPrediction,
		double initialPredictionError, double initialFitness,
		double explorationProbability, bool doGeneticAlgorithmSubsumption,
		bool doActionSetSubsumption);

	Move run(Game& game, Color color);
	Move play(Game& game, Color color) override;
	void afterEnd(Game& game, Color color) override;

	// Retourne l'environnement à partir de l'état donné.
	Environement getEnvironement(Board& board);
	// Retourne l'ensemble de correspondance.
	std::vector<Classifier*> generateMatchSet(Environement& environement, std::vector<Move>& actions);
	// Retourne la correspondance d'un classeur avec un environnement donné.
	int doesMatch(Classifier& classifer, Environement& environement, std::vector<Move>& actions);
	// Retourne le nombre d'actions différentes dans un ensemble de correspondance donné.
	std::vector<Move> differentActions(std::vector<Classifier*>& matchSet);
	// Retourne un classeur généré à partir de l'environnement donné.
	Classifier* generateCoveringClassifier(std::vector<Classifier*>& matchSet, Environement& board, std::vector<Move>& actions);
	// Retourne une action non couverte par l'ensemble de correspondance.
	Move randomActionNotCovered(std::vector<Classifier*>& matchSet, std::vector<Move>& actions);
	// Retourne le tableau de prédiction pour chaque mouvement.
	std::unordered_map<Move, double> generatePredictionArray(std::vector<Classifier*>& matchSet, std::vector<Move>& actions);
	// Retourne la meilleur action possible avec une certaine probabilité.
	Move selectAction(std::unordered_map<Move, double>& predictionArray, std::vector<Move>& actions);
	// Retourne la meilleure action dans l'ensemble de prédiction.
	Move bestAction(std::unordered_map<Move, double>& predictionArray);
	// Retourne l'ensemble d'action à partir de l'ensemble de correspondance et de l'action donné.
	std::vector<Classifier*> generateActionSet(std::vector<Classifier*>& matchSet, Move& action);
	// Retourne la récompense à partir de l'issue d'une partie.
	double getReward(Game& game, Color color);
	// Met à jour l'ensemble d'action donné à partir du profit donné.
	void updateSet(std::vector<Classifier*>& actionSet, double payoff);
	// Met à jour l'aptitude des classeurs de l'ensemble d'action donné.
	void updateFitness(std::vector<Classifier*>& actionSet);
	// Applique l'algorithme génétique sur l'ensemble d'action donné, en prenant en compte l'environnement.
	void geneticAlgorithm(std::vector<Classifier*>& actionSet, Environement& environement, std::vector<Move>& actions);
	// Retourne le classeur servant de parent parmis l'ensemble d'action donné.
	Classifier* selectOffspring(std::vector<Classifier*>& actionSet);
	// Retourne le classeur dérivé du classeur parent donné.
	Classifier* child(Classifier& parent);
	// Applique le croisement entre 2 classeurs.
	void applyCrossover(Classifier& child1, Classifier& chlid2);
	// Applique des mutations sur un classeur donné en prenant en compte l'environnement donné.
	void applyMutation(Classifier& classifier, Environement& environement, std::vector<Move>& actions);
	// Retourne une action différente de l'action donné parmi une liste d'actions donné.
	Move& randomDifferentAction(Move& action, std::vector<Move>& actions);
	// Insère le classeur donné dans la population.
	void insertInPopulation(Classifier& classifier);
	// Supprime un classeur dans la population.
	void deleteFromPopulation();
	// Renvoie le score de suppression d'un classeur.
	double deletionVote(Classifier& classifier);
	// Renvoie le score de suppression d'un classeur.
	double deletionVote(Classifier& classifier, double fitnessSum, int numerositySum);
	// Subsume les classeurs de l'ensemble d'action donné avec le classeur le plus général de cet ensemble d'action.
	void actionSetSubsumption(std::vector<Classifier*>& actionSet);
	// Vérifie si le premier classeur peut subsumber le deuxième classeur.
	int doesSubsume(Classifier& clSub, Classifier& clTos);
	// Vérifie si le classeur est en capacité de subsumer.
	int couldSubsume(Classifier& Classifier);
	// Vérifie si le premier classeur est plus général que le deuxième classeur donné.
	int isMoreGeneral(Classifier& clGen, Classifier& clSpec);
	// Retourne le maximum du tableau de prédiction donné.
	double max(std::unordered_map<Move, double>& predictionArray);

	// Fonctions hérités de la classe parente Player.
	void printState() override;

private:
	// Cardinalité de l'ensemble de correspondance.
	int matchSetCount = 0;
	// Nombre de nouveaux classeurs créees.
	int newClassfiers = 0;
	// Valeur moyenne du tableau de prédiction.
	double predictionArrayAverageValue = 0;
	// Indique si la meilleur action a été prise ou non.
	int isBestAction = 0;
	// Valeur moyenne de la prédiciton de l'ensemble d'action.
	double actionSetAverageValue = 0;
	// Cardinalité de l'ensemble d'action.
	int actionSetCount = 0;
	// Compteur d'occurence de subsumption dans l'ensemble d'action.
	int actionSetSubsumptionCount = 0;
	// Indique si l'algorithme génétique a été appliqué.
	int isGeneticAlgorithmApplied = 0;
	// Compteur d'occurence de subsumption dans l'algorithme génétique.
	int geneticAlgorithmSubsumption = 0;
};

void XCSLearning(int populationSize, double learningRate, double alpha,
	double errorThreshold, double power, double discountFactor,
	int geneticAlgorithmThreshold, double crossoverProbability,
	double mutationProbability, int experiencDeletionThreshold,
	double fitnessDeletionThreshold, int experienceSubsumptionThreshold,
	double jokerProbability, double initialPrediction,
	double initialPredictionError, double initialFitness,
	double explorationProbability, bool doGeneticAlgorithmSubsumption,
	bool doActionSetSubsumption,
	std::string experimentName, int minGame, int maxGame, double drawThreshold);

#pragma once

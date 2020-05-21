#pragma once
#include "Player.h"

// Structure d�finissant le contenu d'un classeur dans XCS.
class Classifier {
public:
	// Condition du classeur, extraite d'un �tat.
	std::array<int, 64> condition;
	// Action pr�conis� par le classeur.
	Move action;
	// Pr�diction du classeur.
	double prediction = 0;
	// Erreur de pr�diction du classeur.
	double predictionError = 0;
	// Aptitude du classeur.
	double fitness = 0;
	// Exp�rience du classeur.
	double experience = 0;
	// Pas de temps depuis la derni�re application d'un algorithme g�n�tique sur le classeur.
	int timeStamp = 0;
	// Taille moyenne des ensembles d'actions dont ce classeur a fait partie.
	double actionSetSize = 0;
	// Num�rosit� du classeur.
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

// Structure d�finissant l'environnement utilis� par XCS pour r�cup�rer la situation.
struct Environement {
	std::array<int, 64> board;
	Environement();
	//~Environement();
};

// Structure d�finissant XCS et son fonctionnement.
class XCS : public Player {
public:
	// Enum�ration d�finissant l'identifiant de la valeur joker, ou valeur de g�n�ralisation.
	enum Joker {
		XCS_Joker = 20
	};

	// Ensemble de population.
	std::vector<Classifier*> population;
	// Ensemble d'action pr�c�dent.
	std::vector<Classifier*> previousActionSet;
	// R�compense pr�c�dente.
	double previousReward = 0;
	// Environnement pr�c�dent.
	Environement previousEnvironement;
	// Actions pr�c�dentes.
	std::vector<Move> previousActions;
	// Pas de temps actuel.
	int currentTime = 0;

	// Taille maximale de la population.
	int populationSize;
	// Taux d'apprentissage.
	double learningRate;
	// Taux d'apprentissage de l'aptitude.
	double alpha;
	// Seuil en dessous lequel une erreur de pr�diction est consid�r� comme nul.
	double errorThreshold;
	// Param�tre utilis� pour le calcul de l'aptitude.
	double power;
	// Facteur d'actualisation.
	double discountFactor;
	// Seuil d'activation du l'algorithme g�n�tique.
	int geneticAlgorithmThreshold;
	// Probabilti� de croisement.
	double crossoverProbability;
	// Probabilit� de mutation.
	double mutationProbability;
	// Seuil de prise en compte de l'exp�rience dans le processus de suppression.
	int experienceDeletionThreshold;
	// Seuil de prise en compte de l'aptitude dans le processsu de suppression.
	double fitnessDeletionThreshold;
	// Seuil d'exp�rience au del� duquel un classeur est �ligile pour subsumer d'autres classeurs.
	int experienceSubsumptionThreshold;
	// Probabilit� d'appliquer une valeur de g�n�ralisation au lieu d'une valeur exacte lors de la constitution d'une condition.
	double jokerProbability;
	// Pr�diction initiale.
	double initialPrediction;
	// Erreur de pr�diction initiale.
	double initialPredictionError;
	// Aptitude initiale.
	double initialFitness;
	// Probabilit� d'utiliser une action al�atoire. 
	double explorationProbability;
	// Condition d'utilisation de la subsumption dans l'algorithme g�n�tique.
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

	// Retourne l'environnement � partir de l'�tat donn�.
	Environement getEnvironement(Board& board);
	// Retourne l'ensemble de correspondance.
	std::vector<Classifier*> generateMatchSet(Environement& environement, std::vector<Move>& actions);
	// Retourne la correspondance d'un classeur avec un environnement donn�.
	int doesMatch(Classifier& classifer, Environement& environement, std::vector<Move>& actions);
	// Retourne le nombre d'actions diff�rentes dans un ensemble de correspondance donn�.
	std::vector<Move> differentActions(std::vector<Classifier*>& matchSet);
	// Retourne un classeur g�n�r� � partir de l'environnement donn�.
	Classifier* generateCoveringClassifier(std::vector<Classifier*>& matchSet, Environement& board, std::vector<Move>& actions);
	// Retourne une action non couverte par l'ensemble de correspondance.
	Move randomActionNotCovered(std::vector<Classifier*>& matchSet, std::vector<Move>& actions);
	// Retourne le tableau de pr�diction pour chaque mouvement.
	std::unordered_map<Move, double> generatePredictionArray(std::vector<Classifier*>& matchSet, std::vector<Move>& actions);
	// Retourne la meilleur action possible avec une certaine probabilit�.
	Move selectAction(std::unordered_map<Move, double>& predictionArray, std::vector<Move>& actions);
	// Retourne la meilleure action dans l'ensemble de pr�diction.
	Move bestAction(std::unordered_map<Move, double>& predictionArray);
	// Retourne l'ensemble d'action � partir de l'ensemble de correspondance et de l'action donn�.
	std::vector<Classifier*> generateActionSet(std::vector<Classifier*>& matchSet, Move& action);
	// Retourne la r�compense � partir de l'issue d'une partie.
	double getReward(Game& game, Color color);
	// Met � jour l'ensemble d'action donn� � partir du profit donn�.
	void updateSet(std::vector<Classifier*>& actionSet, double payoff);
	// Met � jour l'aptitude des classeurs de l'ensemble d'action donn�.
	void updateFitness(std::vector<Classifier*>& actionSet);
	// Applique l'algorithme g�n�tique sur l'ensemble d'action donn�, en prenant en compte l'environnement.
	void geneticAlgorithm(std::vector<Classifier*>& actionSet, Environement& environement, std::vector<Move>& actions);
	// Retourne le classeur servant de parent parmis l'ensemble d'action donn�.
	Classifier* selectOffspring(std::vector<Classifier*>& actionSet);
	// Retourne le classeur d�riv� du classeur parent donn�.
	Classifier* child(Classifier& parent);
	// Applique le croisement entre 2 classeurs.
	void applyCrossover(Classifier& child1, Classifier& chlid2);
	// Applique des mutations sur un classeur donn� en prenant en compte l'environnement donn�.
	void applyMutation(Classifier& classifier, Environement& environement, std::vector<Move>& actions);
	// Retourne une action diff�rente de l'action donn� parmi une liste d'actions donn�.
	Move& randomDifferentAction(Move& action, std::vector<Move>& actions);
	// Ins�re le classeur donn� dans la population.
	void insertInPopulation(Classifier& classifier);
	// Supprime un classeur dans la population.
	void deleteFromPopulation();
	// Renvoie le score de suppression d'un classeur.
	double deletionVote(Classifier& classifier);
	// Renvoie le score de suppression d'un classeur.
	double deletionVote(Classifier& classifier, double fitnessSum, int numerositySum);
	// Subsume les classeurs de l'ensemble d'action donn� avec le classeur le plus g�n�ral de cet ensemble d'action.
	void actionSetSubsumption(std::vector<Classifier*>& actionSet);
	// V�rifie si le premier classeur peut subsumber le deuxi�me classeur.
	int doesSubsume(Classifier& clSub, Classifier& clTos);
	// V�rifie si le classeur est en capacit� de subsumer.
	int couldSubsume(Classifier& Classifier);
	// V�rifie si le premier classeur est plus g�n�ral que le deuxi�me classeur donn�.
	int isMoreGeneral(Classifier& clGen, Classifier& clSpec);
	// Retourne le maximum du tableau de pr�diction donn�.
	double max(std::unordered_map<Move, double>& predictionArray);

	// Fonctions h�rit�s de la classe parente Player.
	void printState() override;

private:
	// Cardinalit� de l'ensemble de correspondance.
	int matchSetCount = 0;
	// Nombre de nouveaux classeurs cr�ees.
	int newClassfiers = 0;
	// Valeur moyenne du tableau de pr�diction.
	double predictionArrayAverageValue = 0;
	// Indique si la meilleur action a �t� prise ou non.
	int isBestAction = 0;
	// Valeur moyenne de la pr�diciton de l'ensemble d'action.
	double actionSetAverageValue = 0;
	// Cardinalit� de l'ensemble d'action.
	int actionSetCount = 0;
	// Compteur d'occurence de subsumption dans l'ensemble d'action.
	int actionSetSubsumptionCount = 0;
	// Indique si l'algorithme g�n�tique a �t� appliqu�.
	int isGeneticAlgorithmApplied = 0;
	// Compteur d'occurence de subsumption dans l'algorithme g�n�tique.
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

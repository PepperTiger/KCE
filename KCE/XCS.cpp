#include "XCS.h"

Classifier::Classifier() {
	for (int i = 0; i < 64; i++)
		condition[i] = 0;
}

Classifier::Classifier(const Classifier& cl) {
	for (int i = 0; i < 64; i++)
		condition[i] = cl.condition[i];
	action = { cl.action.from, cl.action.to, cl.action.promote };
	prediction = cl.prediction;
	predictionError = cl.predictionError;
	fitness = cl.fitness;
	experience = cl.experience;
	timeStamp = cl.timeStamp;
	actionSetSize = cl.actionSetSize;
	numerosity = cl.numerosity;
}

bool Classifier::operator==(const Classifier& cl) const {
	for (int i = 0; i < 64; i++) {
		if (condition[i] != cl.condition[i])
			return false;
	}
	return action.from == cl.action.from &&
		action.to == cl.action.to &&
		action.promote == cl.action.promote &&
		prediction == cl.prediction &&
		predictionError == cl.predictionError &&
		fitness == cl.fitness &&
		experience == cl.experience &&
		timeStamp == cl.timeStamp &&
		actionSetSize == cl.actionSetSize &&
		numerosity == cl.numerosity;
}

size_t std::hash<Classifier>::operator()(Classifier const& cl) const {
	size_t hash = 1;
	std::hash<int> hashInt; std::hash<double> hashDouble;
	for (int i = 0; i < 64; i++)
		hash ^= hashInt(cl.condition[i]) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
	hash ^= hashDouble(cl.prediction) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
	hash ^= hashDouble(cl.predictionError) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
	hash ^= hashDouble(cl.fitness) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
	hash ^= hashDouble(cl.experience) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
	hash ^= hashInt(cl.timeStamp) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
	hash ^= hashDouble(cl.actionSetSize) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
	hash ^= hashInt(cl.action.from) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
	hash ^= hashInt(cl.action.to) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
	hash ^= hashInt(cl.numerosity) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
	return hash;
}

bool std::equal_to<Classifier>::operator()(const Classifier& cl1, const Classifier& cl2) const {
	return cl1 == cl2;
}

Environement::Environement() {
	for (int i = 0; i < 64; i++)
		board[i] = 0;
}

XCS::XCS() {};

XCS::XCS(int populationSize, double learningRate, double alpha,
	double errorThreshold, double power, double discountFactor,
	int geneticAlgorithmThreshold, double crossoverProbability,
	double mutationProbability, int experiencDeletionThreshold,
	double fitnessDeletionThreshold, int experienceSubsumptionThreshold,
	double jokerProbability, double initialPrediction,
	double initialPredictionError, double initialFitness,
	double explorationProbability, bool doGeneticAlgorithmSubsumption,
	bool doActionSetSubsumption) {

	this->populationSize = populationSize;
	this->learningRate = learningRate;
	this->alpha = alpha;
	this->errorThreshold = errorThreshold;
	this->power = power;
	this->discountFactor = discountFactor;
	this->geneticAlgorithmThreshold = geneticAlgorithmThreshold;
	this->crossoverProbability = crossoverProbability;
	this->mutationProbability = mutationProbability;
	this->experienceDeletionThreshold = experiencDeletionThreshold;
	this->fitnessDeletionThreshold = fitnessDeletionThreshold;
	this->experienceSubsumptionThreshold = experienceSubsumptionThreshold;
	this->jokerProbability = jokerProbability;
	this->initialPrediction = initialPrediction;
	this->initialPredictionError = initialPredictionError;
	this->initialFitness = initialFitness;
	this->explorationProbability = explorationProbability;
	this->doGeneticAlgorithmSubsumption = doGeneticAlgorithmSubsumption;
	this->doActionSetSubsumption = doActionSetSubsumption;
}

Move XCS::play(Game& game, Color color) {
	matchSetCount = 0;
	newClassfiers = 0;
	predictionArrayAverageValue = 0;
	isBestAction = 0;
	actionSetAverageValue = 0;
	actionSetCount = 0;
	actionSetSubsumptionCount = 0;
	isGeneticAlgorithmApplied = 0;
	geneticAlgorithmSubsumption = 0;
	return run(game, color);
}

Move XCS::run(Game& game, Color color) {
	currentTime++;
	Environement environnement = getEnvironement(game.board);
	std::vector<Move> actions = game.legalMoves[color];
	std::vector<Classifier*> matchSet = generateMatchSet(environnement, actions);
	std::unordered_map<Move, double> predictionArray = generatePredictionArray(matchSet, actions);
	Move action = selectAction(predictionArray, actions);
	if (learning) {
		std::vector<Classifier*> actionSet = generateActionSet(matchSet, action);
		double reward = 0; //getReward(game, color, action);
		double payoff;
		if (previousActionSet.size()) {
			payoff = previousReward + discountFactor * max(predictionArray);
			updateSet(previousActionSet, payoff);
			geneticAlgorithm(previousActionSet, previousEnvironement, actions);
		}
		previousActionSet = actionSet;
		previousReward = reward;
		previousEnvironement = environnement;
		previousActions = actions;
	}
	return action;
}

void XCS::afterEnd(Game& game, Color color) {
	if (learning) {
		double payoff = getReward(game, color);
		updateSet(previousActionSet, payoff);
		geneticAlgorithm(previousActionSet, previousEnvironement, previousActions);
		previousActionSet.clear();
		previousReward = 0;
	}
}

Environement XCS::getEnvironement(Board& board) {
	Environement environement;
	for (int i = 0; i < 2; i++) {
		unsigned __int64 acc = board.board[i];
		while (acc) {
			int index = BitScanForward64(acc);
			environement.board[index] += 10 * i;
			acc ^= 1ull << index;
		}
	}
	for (int i = 2; i < 8; i++) {
		unsigned __int64 acc = board.board[i];
		while (acc) {
			int index = BitScanForward64(acc);
			environement.board[index] += i;
			acc ^= 1ull << index;
		}
	}
	return environement;
}

std::vector<Classifier*> XCS::generateMatchSet(Environement& environement, std::vector<Move>& actions) {
	std::vector<Classifier*> matchSet;
	for (Classifier* classifier : population) {
		if (doesMatch(*classifier, environement, actions))
			matchSet.push_back(classifier);
	}
	while (differentActions(matchSet).size() < actions.size()) {
		Classifier* newClassifier = generateCoveringClassifier(matchSet, environement, actions);
		population.push_back(newClassifier);
		deleteFromPopulation();
		matchSet.push_back(newClassifier);
	}
	for (Classifier* classifier : matchSet)
		matchSetCount += classifier->numerosity;
	return matchSet;
}

int XCS::doesMatch(Classifier& classifier, Environement& environement, std::vector<Move>& actions) {
	int actionFound = false;
	for (Move action : actions) {
		if (action.from == classifier.action.from && action.to == classifier.action.to && action.promote == classifier.action.promote) {
			actionFound = true;
			break;
		}
	}
	if (!actionFound)
		return false;
	for (int i = 0; i < 64; i++) {
		if (classifier.condition[i] != environement.board[i] && classifier.condition[i] != XCS_Joker)
			return false;
	}
	return true;
}

std::vector<Move> XCS::differentActions(std::vector<Classifier*>& matchSet) {
	std::vector<Move> actionsSaved;
	for (Classifier* classifier : matchSet) {
		int contains = false;
		for (Move action : actionsSaved) {
			contains = action.from == classifier->action.from && action.to == classifier->action.to && action.promote == classifier->action.promote;
			if (contains)
				break;
		}
		if (!contains) actionsSaved.push_back(classifier->action);
	}
	return actionsSaved;
}

Classifier* XCS::generateCoveringClassifier(std::vector<Classifier*>& matchSet, Environement& environement, std::vector<Move>& actions) {
	Classifier* classifier = new Classifier;
	for (int i = 0; i < 64; i++)
		classifier->condition[i] = randDouble() < jokerProbability ? XCS_Joker : environement.board[i];
	classifier->action = randomActionNotCovered(matchSet, actions);
	classifier->prediction = initialPrediction;
	classifier->predictionError = initialPredictionError;
	classifier->fitness = initialFitness;
	classifier->experience = 0;
	classifier->experience = 0;
	classifier->timeStamp = currentTime;
	classifier->actionSetSize = 1;
	classifier->numerosity = 1;
	newClassfiers++;
	return classifier;
}

Move XCS::randomActionNotCovered(std::vector<Classifier*>& matchSet, std::vector<Move>& actions) {
	std::vector<Move> actionsCovered = differentActions(matchSet);
	std::vector<Move> actionsNotCovered;
	for (Move action : actions) {
		int covered = false;
		for (Move actionCovered : actionsCovered) {
			covered = action.from == actionCovered.from && action.to == actionCovered.to && action.promote == actionCovered.promote;
			if (covered) break;
		}
		if (!covered) actionsNotCovered.push_back(action);
	}
	return actionsNotCovered[rand() % actionsNotCovered.size()];
}

std::unordered_map<Move, double> XCS::generatePredictionArray(std::vector<Classifier*>& matchSet, std::vector<Move>& actions) {
	std::unordered_map<Move, double> predictionArray;
	std::unordered_map<Move, double> fitnessSumArray;
	for (Classifier* classifier : matchSet) {
		if (predictionArray.count(classifier->action)) {
			predictionArray[classifier->action] += classifier->prediction * classifier->fitness;
			fitnessSumArray[classifier->action] += classifier->fitness;
		}
		else {
			predictionArray.insert({ classifier->action, classifier->prediction * classifier->fitness });
			fitnessSumArray.insert({ classifier->action, classifier->fitness });
		}
	}
	for (Move action : actions) {
		if (fitnessSumArray[action])
			predictionArray[action] /= fitnessSumArray[action];
	}
	int i = 0; double predictionSum = 0;
	for (std::pair<Move, double> movePrediction : predictionArray) {
		predictionSum += movePrediction.second;
		i++;
	}
	predictionArrayAverageValue = predictionSum / i;
	return predictionArray;
}

Move XCS::selectAction(std::unordered_map<Move, double>& predictionArray, std::vector<Move>& actions) {
	if (randDouble() < explorationProbability) {
		isBestAction = false;
		return actions[rand() % actions.size()];
	}
	else {
		isBestAction = true;
		return bestAction(predictionArray);
	}
}

Move XCS::bestAction(std::unordered_map<Move, double>& predictionArray) {
	double maxValue = -INFINITY;
	std::vector<Move> bestActions;
	for (std::pair<Move, double> actionPrediction : predictionArray) {
		if (actionPrediction.second > maxValue) {
			bestActions.clear();
			maxValue = actionPrediction.second;
		}
		if (actionPrediction.second >= maxValue)
			bestActions.push_back(actionPrediction.first);
	}
	return bestActions[rand() % bestActions.size()];
}

std::vector<Classifier*> XCS::generateActionSet(std::vector<Classifier*>& matchSet, Move& action) {
	std::vector<Classifier*> actionSet;
	int numerositySum = 0; double predictionSum = 0;
	for (Classifier* classifier : matchSet) {
		if (classifier->action.from == action.from && classifier->action.to == action.to && classifier->action.promote == action.promote) {
			actionSet.push_back(classifier);
			numerositySum += classifier->numerosity;
			predictionSum += classifier->prediction;
		}
	}
	actionSetCount = numerositySum;
	actionSetAverageValue = predictionSum / numerositySum;
	return actionSet;
}

double XCS::getReward(Game& game, Color color) {
	if (game.outcome == color)
		return 1;
	else if (game.outcome == (color ^ 1))
		return -1;
	else
		return 0;
}

void XCS::updateSet(std::vector<Classifier*>& actionSet, double payoff) {
	int totalNumerosity = 0;
	for (Classifier* classifier : actionSet)
		totalNumerosity += classifier->numerosity;
	for (Classifier* classifier : actionSet) {
		classifier->experience++;
		if (classifier->experience < 1 / learningRate) {
			classifier->prediction += (payoff - classifier->prediction) / classifier->experience;
			classifier->predictionError += (abs(payoff - classifier->prediction) - classifier->predictionError) / classifier->experience;
			classifier->actionSetSize += (totalNumerosity - classifier->actionSetSize) / classifier->experience;
		}
		else {
			classifier->prediction += learningRate * (payoff - classifier->prediction);
			classifier->predictionError += learningRate * (abs(payoff - classifier->prediction) - classifier->predictionError);
			classifier->actionSetSize += learningRate * (totalNumerosity - classifier->actionSetSize);
		}
	}
	updateFitness(actionSet);
	if (doActionSetSubsumption)
		actionSetSubsumption(actionSet);
}

void XCS::updateFitness(std::vector<Classifier*>& actionSet) {
	double accuracySum = 0;
	std::unordered_map<Classifier, double> accuracyVector;
	for (Classifier* classifier : actionSet) {
		double accuracy;
		if (classifier->predictionError < initialPredictionError)
			accuracy = 1;
		else {
			double d = classifier->predictionError / errorThreshold;
			if (isnan(d) || isinf(d))
				std::cout << "issou";
			double p = pow(d, -power);
			if (isnan(p) || isinf(p))
				std::cout << "issou";
			accuracy = alpha * p;
		}
		accuracyVector.insert({ *classifier, accuracy });
		accuracySum += accuracy * classifier->numerosity;
		if (isnan(accuracy) || isinf(accuracy))
			std::cout << "issou";
		if (isnan(accuracySum) || isinf(accuracySum))
			std::cout << "issou";
	}
	for (Classifier* classifier : actionSet) {
		classifier->fitness += learningRate * (accuracyVector[*classifier] * classifier->numerosity / accuracySum - classifier->fitness);
		if (isnan(classifier->fitness) || isinf(classifier->fitness))
			std::cout << "issou";
	}
}

void XCS::geneticAlgorithm(std::vector<Classifier*>& actionSet, Environement& environement, std::vector<Move>& actions) {
	int timeStampNumSum = 0, numerositySum = 0;
	for (Classifier* classifier : actionSet) {
		timeStampNumSum += classifier->timeStamp * classifier->numerosity;
		numerositySum += classifier->numerosity;
	}
	if (currentTime - timeStampNumSum / numerositySum > geneticAlgorithmThreshold) {
		isGeneticAlgorithmApplied = true;
		for (Classifier* classifier : actionSet)
			classifier->timeStamp = currentTime;
		Classifier* parent1 = selectOffspring(actionSet);
		Classifier* parent2 = selectOffspring(actionSet);
		Classifier* child1 = child(*parent1);
		Classifier* child2 = child(*parent2);
		if (randDouble() < crossoverProbability) {
			applyCrossover(*child1, *child2);
			child1->prediction = (parent1->prediction + parent2->prediction) / 2;
			child1->predictionError = 0.25 * (parent1->predictionError + parent2->predictionError) / 2;
			child1->fitness = 0.1 * (parent1->fitness + parent2->fitness) / 2;
			child2->prediction = child1->prediction;
			child2->predictionError = child1->predictionError;
			child2->fitness = child1->fitness;
		}
		applyMutation(*child1, environement, actions);
		applyMutation(*child2, environement, actions);
		if (doGeneticAlgorithmSubsumption) {
			if (doesSubsume(*parent1, *child1)) {
				geneticAlgorithmSubsumption++;
				parent1->numerosity++;
			}
			else if (doesSubsume(*parent2, *child1)) {
				geneticAlgorithmSubsumption++;
				parent2->numerosity++;
			}
			else
				insertInPopulation(*child1);
			deleteFromPopulation();
			if (doesSubsume(*parent1, *child2)) {
				geneticAlgorithmSubsumption++;
				parent1->numerosity++;
			}
			else if (doesSubsume(*parent2, *child2)) {
				geneticAlgorithmSubsumption++;
				parent2->numerosity++;
			}
			else
				insertInPopulation(*child2);
			deleteFromPopulation();
		}
	}
}

Classifier* XCS::selectOffspring(std::vector<Classifier*>& actionSet) {
	double fitnessSum = 0;
	for (Classifier* classifier : actionSet)
		fitnessSum += classifier->fitness;
	double choicePoint = randDouble() * fitnessSum;
	fitnessSum = 0;
	for (Classifier* classifier : actionSet) {
		fitnessSum += classifier->fitness;
		if (fitnessSum > choicePoint)
			return classifier;
	}
	return actionSet.back();
}

Classifier* XCS::child(Classifier& parent) {
	Classifier* cl = new Classifier(parent);
	cl->numerosity = 1;
	cl->experience = 0;
	return cl;
}

void XCS::applyCrossover(Classifier& child1, Classifier& child2) {
	double x = trunc(randDouble() * 64);
	double y = trunc(randDouble() * 64);
	if (x > y) {
		double acc = x;
		x = y;
		y = acc;
	}
	int i = 0;
	do {
		if (x <= i && i < y) {
			int acc = child1.condition[i];
			child1.condition[i] = child2.condition[i];
			child2.condition[i] = acc;
		}
		i++;
	} while (i < y);
}

void XCS::applyMutation(Classifier& classifier, Environement& environement, std::vector<Move>& actions) {
	int i = 0;
	do {
		if (randDouble() < mutationProbability) {
			classifier.condition[i] = classifier.condition[i] == XCS_Joker ? environement.board[i] : XCS_Joker;
		}
		i++;
	} while (i < 64);
	if (randDouble() < mutationProbability && actions.size() > 1)
		classifier.action = randomDifferentAction(classifier.action, actions);
}

Move& XCS::randomDifferentAction(Move& action, std::vector<Move>& actions) {
	Move chosenAction;
	do {
		chosenAction = actions[rand() % actions.size()];
	} while (chosenAction.from == action.from && chosenAction.to == action.to && chosenAction.promote == action.promote);
	return chosenAction;
}

void XCS::insertInPopulation(Classifier& classifier) {
	for (Classifier* cl : population) {
		int actionEqual = false;
		if (classifier.action.from == cl->action.from && classifier.action.to == cl->action.to && classifier.action.promote == cl->action.promote) {
			actionEqual = true;
		}
		int conditionEqual = true;
		for (int i = 0; i < 64; i++) {
			if (classifier.condition[i] != cl->condition[i]) {
				conditionEqual = false;
				break;
			}
		}
		if (actionEqual && conditionEqual)
			cl->numerosity++;
	}
	population.push_back(&classifier);
}

void XCS::deleteFromPopulation() {
	int totalClassifier = 0; double fitnessSum = 0;
	for (Classifier* classifier : population) {
		totalClassifier += classifier->numerosity;
		fitnessSum += classifier->fitness;
	}
	if (totalClassifier < populationSize)
		return;
	double voteSum = 0;
	std::unordered_map<Classifier, double> deletionVotes;
	for (Classifier* classifier : population) {
		double deletionVoteValue = deletionVote(*classifier, fitnessSum, totalClassifier);
		deletionVotes.insert({ *classifier, deletionVoteValue });
		voteSum += deletionVoteValue;
	}
	double choicePoint = randDouble() * voteSum;
	voteSum = 0;
	int i = 0;
	for (Classifier* classifier : population) {
		voteSum += deletionVotes[*classifier];
		if (voteSum > choicePoint) {
			if (classifier->numerosity > 1)
				classifier->numerosity--;
			else {
				population.erase(population.begin() + i);
				delete classifier;
			}
			return;
		}
		i++;
	}
}

double XCS::deletionVote(Classifier& classifier) {
	double vote = classifier.actionSetSize * classifier.numerosity;
	double fitnessSum = 0, numerositySum = 0;
	for (Classifier* cl : population) {
		fitnessSum += classifier.fitness;
		numerositySum += classifier.numerosity;
	}
	double averageFitnessInPopulation = fitnessSum / numerositySum;
	if (classifier.experience > experienceDeletionThreshold&& classifier.fitness / classifier.numerosity < fitnessDeletionThreshold * averageFitnessInPopulation)
		vote *= averageFitnessInPopulation / (classifier.fitness / classifier.numerosity);
	return vote;
}

double XCS::deletionVote(Classifier& classifier, double fitnessSum, int numerositySum) {
	double vote = classifier.actionSetSize * classifier.numerosity;
	double averageFitnessInPopulation = fitnessSum / numerositySum;
	if (classifier.experience > experienceDeletionThreshold&& classifier.fitness / classifier.numerosity < fitnessDeletionThreshold * averageFitnessInPopulation)
		vote *= averageFitnessInPopulation / (classifier.fitness / classifier.numerosity);
	return vote;
}

void XCS::actionSetSubsumption(std::vector<Classifier*>& actionSet) {
	Classifier* classifier = NULL;
	for (Classifier* cl : actionSet) {
		if (couldSubsume(*cl)) {
			if (classifier == NULL || isMoreGeneral(*cl, *classifier))
				classifier = cl;
		}
	}
	std::vector<Classifier*> actionSetCopy(actionSet);
	if (classifier != NULL) {
		int i = 0;
		for (Classifier* cl : actionSetCopy) {
			if (isMoreGeneral(*classifier, *cl)) {
				actionSetSubsumptionCount++;
				classifier->numerosity += cl->numerosity;
				actionSet.erase(std::find(actionSet.begin(), actionSet.end(), cl));
				population.erase(std::find(population.begin(), population.end(), cl));
				delete cl;
				i--;
			}
			i++;
		}
	}
}

int XCS::doesSubsume(Classifier& clSub, Classifier& clTos) {
	if (clSub.action.from == clTos.action.from && clSub.action.to == clTos.action.to && clSub.action.promote == clTos.action.promote) {
		if (couldSubsume(clSub)) {
			if (isMoreGeneral(clSub, clTos))
				return true;
		}
	}
	return false;
}

int XCS::couldSubsume(Classifier& classifier) {
	if (classifier.experience > experienceSubsumptionThreshold) {
		if (classifier.predictionError < initialPredictionError)
			return true;
	}
	return false;
}

int XCS::isMoreGeneral(Classifier& clGen, Classifier& clSpec) {
	if (clGen.action.from != clSpec.action.from || clGen.action.to != clSpec.action.to || clGen.action.promote != clSpec.action.promote)
		return false;
	int clGenJokerCount = 0, clSpecJokerCount = 0;
	for (int i = 0; i < 64; i++) {
		if (clGen.condition[i] == XCS_Joker) clGenJokerCount++;
		if (clSpec.condition[i] == XCS_Joker) clSpecJokerCount++;
	}
	if (clGenJokerCount <= clSpecJokerCount)
		return false;
	int i = 0;
	do {
		if (clGen.condition[i] != XCS_Joker && clGen.condition[i] != clSpec.condition[i])
			return false;
		i++;
	} while (i < 64);
	return true;
}

double XCS::max(std::unordered_map<Move, double>& predictionArray) {
	double maxValue = -INFINITY;
	for (std::pair<Move, double> movePrediction : predictionArray) {
		if (movePrediction.second > maxValue)
			maxValue = movePrediction.second;
	}
	return maxValue;
}

void XCS::printState() {
	std::cout << "Current time : " << currentTime << std::endl;
	std::cout << "Match set count : " << matchSetCount << "/" << population.size() << std::endl;
	std::cout << "New classifiers : " << newClassfiers << std::endl;
	std::cout << "Prediction array average value : " << predictionArrayAverageValue << std::endl;
	std::cout << "Action : " << (isBestAction ? "Best" : "Random") << std::endl;
	std::cout << "Action set size : " << actionSetCount << "classifiers with average value : " << actionSetAverageValue << std::endl;
	if (doActionSetSubsumption)
		std::cout << "Action set subsumption : " << actionSetSubsumptionCount << std::endl;
	if (isGeneticAlgorithmApplied)
		std::cout << "Genetic algorithm applied : " << geneticAlgorithmSubsumption << "subsumptions" << std::endl;
}

void XCSLearning(int populationSize, double learningRate, double alpha,
	double errorThreshold, double power, double discountFactor,
	int geneticAlgorithmThreshold, double crossoverProbability,
	double mutationProbability, int experiencDeletionThreshold,
	double fitnessDeletionThreshold, int experienceSubsumptionThreshold,
	double jokerProbability, double initialPrediction,
	double initialPredictionError, double initialFitness,
	double explorationProbability, bool doGeneticAlgorithmSubsumption,
	bool doActionSetSubsumption,
	std::string experimentName, int minGame, int maxGame, double drawThreshold) {

	XCS whitePlayer = XCS(populationSize, learningRate, alpha,
		errorThreshold, power, discountFactor,
		geneticAlgorithmThreshold, crossoverProbability,
		mutationProbability, experiencDeletionThreshold,
		fitnessDeletionThreshold, experienceSubsumptionThreshold,
		jokerProbability, initialPrediction,
		initialPredictionError, initialFitness,
		explorationProbability, doGeneticAlgorithmSubsumption,
		doActionSetSubsumption);
	XCS blackPlayer = XCS(populationSize, learningRate, alpha,
		errorThreshold, power, discountFactor,
		geneticAlgorithmThreshold, crossoverProbability,
		mutationProbability, experiencDeletionThreshold,
		fitnessDeletionThreshold, experienceSubsumptionThreshold,
		jokerProbability, initialPrediction,
		initialPredictionError, initialFitness,
		explorationProbability, doGeneticAlgorithmSubsumption,
		doActionSetSubsumption);
	std::shared_ptr<Player> white = std::shared_ptr<Player>(&whitePlayer);
	std::shared_ptr<Player> black = std::shared_ptr<Player>(&blackPlayer);
	int i = 0, gameEndSum = 0;
	int gamesEnd[3] = { 0, 0, 0 };
	do {
		Game game = Game(white, black);
		game.id = i;
		int outcome = game.runGame();
		int index = outcome == C_White ? 0 : outcome == C_Black ? 1 : 2;
		std::cout << "Game Id" << i << "completed : " << index << std::endl;
		gamesEnd[index]++;
		i++;
	} while (i < minGame || (minGame <= i && i < maxGame && (gamesEnd[2] / (gameEndSum) <= drawThreshold)));
}
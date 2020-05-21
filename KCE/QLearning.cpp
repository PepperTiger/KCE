#include "QLearning.h"

QLearning::QLearning() {};

QLearning::QLearning(double explorationProbability, double learningRate, double discountFactor) {
	this->explorationProbability = explorationProbability;
	this->learningRate = learningRate;
	this->discountFactor = discountFactor;
}

Move QLearning::play(Game& game, Color color) {
	return run(game, color);
}

Move QLearning::run(Game& game, Color color) {
	getEnvironement(game, color);
	return chooseAction();
}

void QLearning::afterOpponentMove(Game& game, Color color, Move move) {
	if (game.turnCount != 1 || color != C_Black)
		updateQTable(game, color);
}

void QLearning::afterEnd(Game& game, Color color) {
	if (game.currentPlayer == color)
		updateQTable(game, color);
	knownNextState = false;
	reward = 0;
	oldQualityValue = 0;
	optimalNextQualityValue = 0;
	newQualityValue = 0;
}

void QLearning::getEnvironement(Game& game, Color color) {
	knownState = true;
	currentQState = Board(game.board);
	if (!qTable.count(Board(game.board))) {
		knownState = false;
		qTable.insert({ currentQState, std::unordered_map<Move, double>() });
		for (Move move : game.legalMoves[color])
			qTable[currentQState].insert({ Move(move), 0 });
	}
}

Move QLearning::chooseAction() {
	if (randDouble() < explorationProbability) {
		bestActionChosen = false;
		int i = 0, index = rand() % qTable[currentQState].size();
		for (std::pair<Move, double> qActionValue : qTable[currentQState]) {
			if (i == index) {
				chosenQAction = Move(qActionValue.first);
				break;
			}
			i++;
		}
	}
	else {
		chosenQAction = Move(bestAction());
		bestActionChosen = true;
	}
	currentQValue = qTable[currentQState][chosenQAction];
	return chosenQAction;
}

Move QLearning::bestAction() {
	std::vector<Move> bestActions;
	double bestValue = -INFINITY;
	for (std::pair<Move, double> qActionValue : qTable[currentQState]) {
		double maxNextStateValue = qActionValue.second;
		if (maxNextStateValue > bestValue) {
			bestValue = maxNextStateValue;
			bestActions.clear();
		}
		if (maxNextStateValue >= bestValue)
			bestActions.push_back(qActionValue.first);
	}
	bestActionQValue = bestValue;
	return bestActions[rand() % bestActions.size()];
}

void QLearning::updateQTable(Game& game, Color color) {
	knownNextState = true;
	nextQState = Board(game.board);
	reward = getReward(game, color);
	oldQualityValue = currentQValue;
	optimalNextQualityValue = optimalNextValue();
	double temporalDifference = reward + discountFactor * optimalNextQualityValue - oldQualityValue;
	newQualityValue = oldQualityValue + learningRate * temporalDifference;
	qTable[currentQState][chosenQAction] = newQualityValue;
}

double QLearning::getReward(Game& game, Color color) {
	if (game.outcome == O_NotDone ||
		game.outcome == O_DrawByRepetion ||
		game.outcome == O_DrawByNoMove ||
		game.outcome == O_DrawByBlocked)
		return 0;
	else if (game.outcome == color)
		return 1;
	else
		return -1;
}

double QLearning::optimalNextValue() {
	if (!qTable.count(nextQState))
		return 0;
	else {
		std::unordered_map<Move, double> nextQValues = qTable[nextQState];
		double maxValue = -INFINITY;
		for (std::pair<Move, double> nextQValue : nextQValues) {
			if (nextQValue.second > maxValue)
				maxValue = nextQValue.second;
		}
		return maxValue;
	}
}

void QLearning::printState() {
	if (knownNextState) {
		std::cout << "*** Q-Table previous update ***" << std::endl;
		std::cout << "Reward : " << reward << std::endl;
		std::cout << "Old Q-value : " << oldQualityValue << std::endl;
		std::cout << "Optimal next Q-value : " << optimalNextQualityValue << std::endl;
		std::cout << "New Q-value : " << newQualityValue << std::endl;
	}
	std::cout << "*** Q-state and Q-action ***" << std::endl;
	std::cout << "State : " << (knownState ? "Known state" : "Unknown state") << std::endl;
	std::cout << "Action : ";
	if (bestActionChosen)
		std::cout << "Best action chosen with Q-Value : " << bestActionChosen;
	else
		std::cout << "Random action";
	std::cout << std::endl;
}

void RunQLearning(double learningRate, double discountFactor, double explorationProbability,
	int minGames, int maxGames) {
	QLearning whitePlayer(explorationProbability, learningRate, discountFactor);
	QLearning blackPlayer(explorationProbability, learningRate, discountFactor);
	std::shared_ptr<Player> white(&whitePlayer);
	std::shared_ptr<Player> black(&blackPlayer);
	int i = 0;
	int gamesEnd[3] = { 0,0,0 };
	do {
		Game game(white, black);
		game.id = i;
		int outcome = game.runGame();
		int index = outcome == C_White ? 0 : outcome == C_Black ? 1 : 2;
		std::cout << "Game Id " << i << " completed : " << index << std::endl;
		i++;
	} while (i < minGames || (minGames <= i && i < maxGames));
}
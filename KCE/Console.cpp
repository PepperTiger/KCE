#include "Console.h"

void runKCE() {
	srand(time(NULL));
	std::cout << "**************************************************" << std::endl;
	std::cout << "* Kasparov Chess Engine - Training & Test Progam *" << std::endl;
	std::cout << "**************************************************" << std::endl;
	std::cout << "Welcome to the Kasparov Chess Engine training and test program." << std::endl;
	std::cout << "This program is used to train learning AIs and test them against each others." << std::endl;
	unsigned int selection = 0;
	unsigned int moveRuleCounter = 50;
	unsigned int foldRepetitionCounter = 3;
	do {
		if (selection > 2) {
			//system("cls");
			std::cout << "Wrong selection, try again" << std::endl;
		}
		std::cout << "Choose one of the few options : " << std::endl;
		std::cout << " 0. Modify move rule and fold repetition counter (" << moveRuleCounter << "-" << foldRepetitionCounter << ")." << std::endl;
		std::cout << " 1. Run learning procedure." << std::endl;
		std::cout << " 2. Run test procedure." << std::endl;
		std::cout << " 3. Exit." << std::endl;
		std::cout << "Type the ID of the chosen option : ";
		std::cin >> selection;
		if (selection == 0) {
			//system("cls");
			modifyCounter(moveRuleCounter, foldRepetitionCounter);
			//system("cls");
		}
		else if (selection == 1) {
			//system("cls");
			prepareLearningProcedure(moveRuleCounter, foldRepetitionCounter);
			//system("cls");
		}
		else if (selection == 2) {
			//system("cls");
			prepareTestProcedure(moveRuleCounter, foldRepetitionCounter);
			//system("cls");
		}
		else if (selection == 3) {
			break;
		}
	} while (1);
}

void modifyCounter(unsigned int& moveRuleCounter, unsigned int& foldRepetitionCounter) {
	std::cout << "Type the move rule counter : ";
	std::cin >> moveRuleCounter;
	std::cout << "Type the fold repetition counter : ";
	std::cin >> foldRepetitionCounter;
}

void prepareLearningProcedure(unsigned int moveRuleCounter, unsigned int foldRepetitionCounter) {
	int aiSelection = 0;
	do {
		if (aiSelection > 1) {
			//system("cls");
			std::cout << "Wrong selection, try again" << std::endl;
		}
		std::cout << "Choose the AI on which to apply the learning procedure : " << std::endl;
		std::cout << " 0. QLearning." << std::endl;
		std::cout << " 1. XCS. " << std::endl;
		std::cout << "Type the ID of the chosen option : ";
		std::cin >> aiSelection;
		if (aiSelection == 0) {
			//system("cls");
			prepareQLearningProcedure(moveRuleCounter, foldRepetitionCounter);
			std::string enter;
			std::cout << "Learning done. Type anything to exit : ";
			std::cin >> enter;
			break;
		}
		else if (aiSelection == 1) {
			//system("cls");
			prepareXCSLearningProcedure(moveRuleCounter, foldRepetitionCounter);
			std::string enter;
			std::cout << "Learning done. Type anything to exit : ";
			std::cin >> enter;
			break;
		}
	} while (1);
}

void prepareQLearningProcedure(unsigned int moveRuleCounter, unsigned int foldRepetitionCounter) {
	std::string profileName = "trash";
	double explorationProbability = 0;
	double learningRate = 0;
	double discountFactor = 0;
	unsigned int selection = 0;
	do {
		if (selection > 1) {
			std::cout << "Wrong selection, try again" << std::endl;
		}
		std::cout << "Choose the QLearning learning profile (learning rate, discount factor and exploration probability) : " << std::endl;
		std::cout << " 0. Classic QLearning Profile (ep = 0.33, lr = 0.1, df = 0.71)." << std::endl;
		std::cout << " 1. Custom profile." << std::endl;
		std::cin >> selection;
		//system("cls");
		if (selection == 0) {
			profileName = "classic_profile";
			explorationProbability = 0.33;
			learningRate = 0.1;
			discountFactor = 0.71;
		}
		else if (selection == 1) {
			do {
				if (profileName == "") {
					//system("cls");
					std::cout << "Wrong selection, try again" << std::endl;
				}
				std::cout << "Type profile name : ";
				std::cin >> profileName;
			} while (profileName == "");
			do {
				if (explorationProbability < 0 || explorationProbability > 1) {
					//system("cls");
					std::cout << "Wrong selection, try again" << std::endl;
				}
				std::cout << "Type the exploration probability (between 0 and 1 included) : ";
				std::cin >> explorationProbability;
			} while (explorationProbability < 0 || explorationProbability > 1);
			//system("cls");
			do {
				if (learningRate < 0 || learningRate > 1) {
					//system("cls");
					std::cout << "Wrong selection, try again" << std::endl;
				}
				std::cout << "Type the learning rate (between 0 and 1 included) : ";
				std::cin >> learningRate;
			} while (learningRate < 0 || learningRate > 1);
			//system("cls");
			do {
				if (discountFactor < 0 || discountFactor > 1) {
					//system("cls");
					std::cout << "Wrong selection, try again" << std::endl;
				}
				std::cout << "Type the discount factor (between 0 and 1 included) : ";
				std::cin >> discountFactor;
			} while (discountFactor < 0 || discountFactor > 1);
			//system("cls");
		}
	} while (selection > 1);
	unsigned int minGames;
	std::cout << "Type the minimum number of games before checking for convergence : ";
	std::cin >> minGames;
	//system("cls");
	bool display;
	std::cout << "Type the display mode (0 = no display, any other value = displaying) : ";
	std::cin >> display;
	//system("cls");
	runQLearningProcedure(profileName, explorationProbability, learningRate, discountFactor,
		minGames, display,
		moveRuleCounter, foldRepetitionCounter);

}

void runQLearningProcedure(std::string profileName,
	double explorationProbability, double learningRate, double discountFactor,
	unsigned int minGames, bool display,
	unsigned int moveRuleCounter, unsigned int foldRepetitionCounter) {
	//QLearning whitePlayer(explorationProbability, learningRate, discountFactor);
	//QLearning blackPlayer(explorationProbability, learningRate, discountFactor);
	std::shared_ptr<Player> white(new QLearning(explorationProbability, learningRate, discountFactor));
	std::shared_ptr<Player> black(new QLearning(explorationProbability, learningRate, discountFactor));
	int i = 0;
	while (i < minGames && stopCondition()) {
		Game game(white, black, display);
		game.id = i;
		game.moveCounterMax = moveRuleCounter;
		game.foldRepetionMax = foldRepetitionCounter;
		Outcome outcome = (Outcome)game.runGame();
		saveQLearning((QLearning&)(*white), profileName + "_white");
		saveQLearning((QLearning&)(*black), profileName + "_black");
		std::cout << "Game " << i << " done, status : ";
		switch (outcome)
		{
		case O_White:
			std::cout << "white wins";
			break;
		case O_Black:
			std::cout << "black wins";
			break;
		case O_DrawByBlocked:
			std::cout << "draw by king blocked";
			break;
		case O_DrawByNoMove:
			std::cout << "draw by no important move";
			break;
		case O_DrawByRepetion:
			std::cout << "draw by fold repetition";
			break;
		default:
			std::cout << "unknown";
			break;
		}
		std::cout << std::endl;
		i++;
	}
}

void saveQLearning(QLearning& qLearning, std::string profileName) {
	std::filesystem::create_directory("Saves");
	std::filesystem::create_directory("Saves/QLearning");
	profileName = std::regex_replace(profileName, std::regex(" "), "_");
	std::string filePath = "Saves/QLearning/" + profileName + ".ql";
	std::ofstream out(filePath);
	size_t size = qLearning.qTable.size();
	out.write((char*)&size, sizeof(size_t));
	for (std::pair<Board, std::unordered_map<Move, double>> bmd : qLearning.qTable) {
		out.write((char*)&bmd.first, sizeof(Board));
		size_t size2 = bmd.second.size();
		out.write((char*)&size2, sizeof(size_t));
		for (std::pair<Move, double> md : bmd.second) {
			out.write((char*)&md.first, sizeof(Move));
			out.write((char*)&md.second, sizeof(double));
		}
	}
	out.write((char*)&qLearning.explorationProbability, sizeof(double));
	out.write((char*)&qLearning.learningRate, sizeof(double));
	out.write((char*)&qLearning.discountFactor, sizeof(double));
	out.close();
}

QLearning loadQLearning(std::string profileName) {
	profileName = std::regex_replace(profileName, std::regex(" "), "_");
	std::string filePath = "Saves/QLearning/" + profileName + ".ql";
	std::ifstream in(filePath);
	QLearning qLearning;
	size_t size;
	in.read((char*)&size, sizeof(size_t));
	for (size_t i = 0; i < size; i++) {
		Board board;
		in.read((char*)&board, sizeof(Board));
		std::unordered_map<Move, double> md;
		size_t size2;
		in.read((char*)&size2, sizeof(size_t));
		for (size_t j = 0; j < size2; j++) {
			Move move;
			in.read((char*)&move, sizeof(Move));
			double score;
			in.read((char*)&score, sizeof(double));
			md.insert({ move, score });
		}
		qLearning.qTable.insert({ board, md });
	}
	in.read((char*)&qLearning.explorationProbability, sizeof(double));
	in.read((char*)&qLearning.learningRate, sizeof(double));
	in.read((char*)&qLearning.discountFactor, sizeof(double));
	in.close();
	return qLearning;
}

void prepareXCSLearningProcedure(unsigned int moveRuleCounter, unsigned int foldRepetitionCounter) {
	std::string profileName;
	unsigned int populationSize = 0;
	double learningRate = 0;
	double alpha = 0;
	double errorThreshold = 0;
	double power = 0;
	double discountFactor = 0;
	unsigned int geneticAlgorithmThreshold = 0;
	double crossoverProbability = 0;
	double mutationProbability = 0;
	unsigned int experienceDeletionThreshold = 0;
	double fitnessDeletionThreshold = 0;
	unsigned int experienceSubsumptionThreshold = 0;
	double jokerProbability = 0;
	double initialPrediction = 0;
	double initialPredictionError = 0;
	double initialFitness = 0;
	double explorationProbability = 0;
	bool doGeneticAlgorithmSubsumption = 0;
	bool doActionSetSubsumption = 0;
	unsigned int selection = 0;
	std::string trash;
	do {
		if (selection > 1) {
			std::cout << "Wrong selection, try again" << std::endl;
		}
		std::cout << "Choose the QLearning learning profile (learning rate, discount factor and exploration probability) : " << std::endl;
		std::cout << " 0. Classic XCS Profile (ps = INT_MAX, lr = 0.1, alpha = 0.1," << std::endl;
		std::cout << "                         et = 0.01, power = 5, df = 0.71," << std::endl;
		std::cout << "                         gat = 25, cp = 1, mp = 0.05," << std::endl;
		std::cout << "                         est = 20, jp = 0.75, ip = 0," << std::endl;
		std::cout << "                         ipe = 0.01, if = 0, ep = 0.5," << std::endl;
		std::cout << "                         dgas = true, das = true)" << std::endl;
		std::cout << " 1. Custom profile." << std::endl;
		std::cin >> selection;
		//system("cls");
		if (selection == 0) {
			profileName = "classic_profile";
			populationSize = INT_MAX;
			learningRate = 0.1;
			alpha = 0.1;
			errorThreshold = 0.01;
			power = 5;
			discountFactor = 0.71;
			geneticAlgorithmThreshold = 25;
			crossoverProbability = 1;
			mutationProbability = 0.05;
			experienceDeletionThreshold = 20;
			fitnessDeletionThreshold = 0.1;
			experienceSubsumptionThreshold = 20;
			jokerProbability = 0.921875;
			initialPrediction = 0;
			initialPredictionError = 0.01;
			initialFitness = 0;
			explorationProbability = 0.5;
			doGeneticAlgorithmSubsumption = true;
			doActionSetSubsumption = true;
		}
		else if (selection == 1) {
			do {
				if (profileName == "") {
					//system("cls");
					std::cout << "Wrong selection, try again" << std::endl;
				}
				std::cout << "Type profile name : ";
				std::cin >> profileName;
			} while (profileName == "");
			do {
				if (populationSize == 0 || populationSize > INT_MAX) {
					//system("cls");
					std::cout << "Wrong selection, try again" << std::endl;
				}
				std::cout << "Type the population size (between 1 and INT_MAX included) : ";
				std::cin >> populationSize;
			} while (populationSize == 0 || populationSize > INT_MAX);
			//system("cls");
			do {
				if (learningRate < 0 || learningRate > 1) {
					//system("cls");
					std::cout << "Wrong selection, try again" << std::endl;
				}
				std::cout << "Type the learning rate (between 0 and 1 included) : ";
				std::cin >> learningRate;
			} while (learningRate < 0 || learningRate > 1);
			//system("cls");
			do {
				if (alpha < 0 || alpha > 1) {
					//system("cls");
					std::cout << "Wrong selection, try again" << std::endl;
				}
				std::cout << "Type the alpha learning rate (between 0 and 1 included) : ";
				std::cin >> alpha;
			} while (alpha < 0 || alpha > 1);
			//system("cls");
			do {
				if (errorThreshold < 0 || errorThreshold > 1) {
					//system("cls");
					std::cout << "Wrong selection, try again" << std::endl;
				}
				std::cout << "Type the error threshold (between 0 and 1 included) : ";
				std::cin >> errorThreshold;
			} while (errorThreshold < 0 || errorThreshold > 1);
			//system("cls");
			do {
				if (power < 0) {
					//system("cls");
					std::cout << "Wrong selection, try again" << std::endl;
				}
				std::cout << "Type the power (more than 0 included) : ";
				std::cin >> power;
			} while (power < 0);
			//system("cls");
			do {
				if (discountFactor < 0 || discountFactor > 1) {
					//system("cls");
					std::cout << "Wrong selection, try again" << std::endl;
				}
				std::cout << "Type the discount factor (between 0 and 1 included) : ";
				std::cin >> discountFactor;
			} while (discountFactor < 0 || discountFactor > 1);
			do {
				if (geneticAlgorithmThreshold > INT_MAX) {
					//system("cls");
					std::cout << "Wrong selection, try again" << std::endl;
				}
				std::cout << "Type the genetic algorithm threshold (more than 0 included) : ";
				std::cin >> geneticAlgorithmThreshold;
			} while (geneticAlgorithmThreshold > INT_MAX);
			do {
				if (crossoverProbability < 0 || crossoverProbability > 1) {
					//system("cls");
					std::cout << "Wrong selection, try again" << std::endl;
				}
				std::cout << "Type the crossover probability (between 0 and 1 included) : ";
				std::cin >> crossoverProbability;
			} while (crossoverProbability < 0 || crossoverProbability > 1);
			do {
				if (mutationProbability < 0 || mutationProbability > 1) {
					//system("cls");
					std::cout << "Wrong selection, try again" << std::endl;
				}
				std::cout << "Type the mutation probability (between 0 and 1 included) : ";
				std::cin >> mutationProbability;
			} while (mutationProbability < 0 || mutationProbability > 1);
			do {
				if (experienceDeletionThreshold > INT_MAX) {
					//system("cls");
					std::cout << "Wrong selection, try again" << std::endl;
				}
				std::cout << "Type the experience deletion threshold (more than 0 included) : ";
				std::cin >> experienceDeletionThreshold;
			} while (experienceDeletionThreshold > INT_MAX);
			do {
				if (experienceSubsumptionThreshold > INT_MAX) {
					//system("cls");
					std::cout << "Wrong selection, try again" << std::endl;
				}
				std::cout << "Type the experience subsumption threshold (more than 0 included) : ";
				std::cin >> experienceSubsumptionThreshold;
			} while (experienceSubsumptionThreshold > INT_MAX);
			do {
				if (fitnessDeletionThreshold < 0 || fitnessDeletionThreshold > 1) {
					//system("cls");
					std::cout << "Wrong selection, try again" << std::endl;
				}
				std::cout << "Type the fitness deletion threshold (between 0 and 1 included) : ";
				std::cin >> fitnessDeletionThreshold;
			} while (fitnessDeletionThreshold < 0 || fitnessDeletionThreshold > 1);
			do {
				if (jokerProbability < 0 || jokerProbability > 1) {
					//system("cls");
					std::cout << "Wrong selection, try again" << std::endl;
				}
				std::cout << "Type the joker probability (between 0 and 1 included) : ";
				std::cin >> jokerProbability;
			} while (jokerProbability < 0 || jokerProbability > 1);
			do {
				if (initialPrediction < -1 || initialPrediction > 1) {
					//system("cls");
					std::cout << "Wrong selection, try again" << std::endl;
				}
				std::cout << "Type the initial prediction (between -1 and 1 included) : ";
				std::cin >> initialPrediction;
			} while (initialPrediction < -1 || initialPrediction > 1);
			do {
				if (initialPredictionError < 0 || initialPredictionError > 1) {
					//system("cls");
					std::cout << "Wrong selection, try again" << std::endl;
				}
				std::cout << "Type the initial prediction error (between 0 and 1 included) : ";
				std::cin >> initialPredictionError;
			} while (initialPredictionError < 0 || initialPredictionError > 1);
			do {
				if (initialFitness < 0 || initialFitness > 1) {
					//system("cls");
					std::cout << "Wrong selection, try again" << std::endl;
				}
				std::cout << "Type the initial fitness (between 0 and 1 included) : ";
				std::cin >> initialFitness;
			} while (initialFitness < 0 || initialFitness > 1);
			do {
				if (explorationProbability < 0 || explorationProbability > 1) {
					//system("cls");
					std::cout << "Wrong selection, try again" << std::endl;
				}
				std::cout << "Type the exploration probability (between 0 and 1 included) : ";
				std::cin >> explorationProbability;
			} while (explorationProbability < 0 || explorationProbability > 1);
			do {
				if (doGeneticAlgorithmSubsumption < 0) {
					//system("cls");
					std::cout << "Wrong selection, try again" << std::endl;
				}
				std::cout << "Type the subsumption in genetic algorithm condition (0 = no, else yes) : ";
				std::cin >> doGeneticAlgorithmSubsumption;
				std::cin >> trash;
			} while (doGeneticAlgorithmSubsumption < 0);
			do {
				if (doActionSetSubsumption < 0) {
					//system("cls");
					std::cout << "Wrong selection, try again" << std::endl;
				}
				std::cout << "Type the subsumption in action set condition (0 = no, else yes) : ";
				std::cin >> doActionSetSubsumption;
				std::cin >> trash;
			} while (doActionSetSubsumption < 0);
		}
	} while (selection > 1);
	unsigned int minGames;
	std::cout << "Type the minimum number of games before checking for convergence : ";
	std::cin >> minGames;
	//system("cls");
	bool display;
	std::cout << "Type the display mode (0 = no display, any other value = displaying) : ";
	std::cin >> display;
	//system("cls");
	profileName += "_";
	profileName += std::to_string(minGames);
	runXCSLearningProcedure(profileName,
		populationSize, learningRate, alpha,
		errorThreshold, power, discountFactor,
		geneticAlgorithmThreshold, crossoverProbability,
		mutationProbability, experienceDeletionThreshold,
		fitnessDeletionThreshold, experienceSubsumptionThreshold,
		jokerProbability, initialPrediction,
		initialPredictionError, initialFitness,
		explorationProbability, doGeneticAlgorithmSubsumption,
		doActionSetSubsumption,
		minGames, display,
		moveRuleCounter, foldRepetitionCounter);
}

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
	unsigned int moveRuleCounter, unsigned int foldRepetitionCounter) {
	std::shared_ptr<Player> white = std::shared_ptr<Player>(new XCS(populationSize, learningRate, alpha,
		errorThreshold, power, discountFactor,
		geneticAlgorithmThreshold, crossoverProbability,
		mutationProbability, experiencDeletionThreshold,
		fitnessDeletionThreshold, experienceSubsumptionThreshold,
		jokerProbability, initialPrediction,
		initialPredictionError, initialFitness,
		explorationProbability, doGeneticAlgorithmSubsumption,
		doActionSetSubsumption));
	std::shared_ptr<Player> black = std::shared_ptr<Player>(new XCS(populationSize, learningRate, alpha,
		errorThreshold, power, discountFactor,
		geneticAlgorithmThreshold, crossoverProbability,
		mutationProbability, experiencDeletionThreshold,
		fitnessDeletionThreshold, experienceSubsumptionThreshold,
		jokerProbability, initialPrediction,
		initialPredictionError, initialFitness,
		explorationProbability, doGeneticAlgorithmSubsumption,
		doActionSetSubsumption));
	int i = 0;
	while (i < minGames && stopCondition()) {
		Game game(white, black, display);
		game.id = i;
		game.moveCounterMax = moveRuleCounter;
		game.foldRepetionMax = foldRepetitionCounter;
		Outcome outcome = (Outcome)game.runGame();
		saveXCS((XCS&)*white, profileName + "_white");
		saveXCS((XCS&)*black, profileName + "_black");
		std::cout << "Game " << i << " done, status : ";
		switch (outcome)
		{
		case O_White:
			std::cout << "white wins";
			break;
		case O_Black:
			std::cout << "black wins";
			break;
		case O_DrawByBlocked:
			std::cout << "draw by king blocked";
			break;
		case O_DrawByNoMove:
			std::cout << "draw by no important move";
			break;
		case O_DrawByRepetion:
			std::cout << "draw by fold repetition";
			break;
		default:
			std::cout << "unknown";
			break;
		}
		std::cout << std::endl;
		i++;
	}
}

void saveXCS(XCS& xcs, std::string profileName) {
	std::filesystem::create_directory("Saves");
	std::filesystem::create_directory("Saves/XCS");
	profileName = std::regex_replace(profileName, std::regex(" "), "_");
	std::string filePath = "./Saves/XCS/" + profileName + ".xcs";
	std::filesystem::remove(filePath);
	std::ofstream out(filePath, std::ofstream::out | std::ofstream::binary | std::ofstream::app);
	out.write((char*)&xcs.currentTime, sizeof(xcs.currentTime));
	out.write((char*)&xcs.populationSize, sizeof(xcs.populationSize));
	out.write((char*)&xcs.learningRate, sizeof(xcs.learningRate));
	out.write((char*)&xcs.alpha, sizeof(xcs.alpha));
	out.write((char*)&xcs.errorThreshold, sizeof(xcs.errorThreshold));
	out.write((char*)&xcs.power, sizeof(xcs.power));
	out.write((char*)&xcs.discountFactor, sizeof(xcs.discountFactor));
	out.write((char*)&xcs.geneticAlgorithmThreshold, sizeof(&xcs.geneticAlgorithmThreshold));
	out.write((char*)&xcs.crossoverProbability, sizeof(xcs.crossoverProbability));
	out.write((char*)&xcs.mutationProbability, sizeof(xcs.mutationProbability));
	out.write((char*)&xcs.experienceDeletionThreshold, sizeof(xcs.experienceDeletionThreshold));
	out.write((char*)&xcs.fitnessDeletionThreshold, sizeof(xcs.fitnessDeletionThreshold));
	out.write((char*)&xcs.experienceSubsumptionThreshold, sizeof(xcs.experienceSubsumptionThreshold));
	out.write((char*)&xcs.jokerProbability, sizeof(xcs.jokerProbability));
	out.write((char*)&xcs.initialPrediction, sizeof(xcs.initialPrediction));
	out.write((char*)&xcs.initialPredictionError, sizeof(xcs.initialPredictionError));
	out.write((char*)&xcs.initialFitness, sizeof(xcs.initialFitness));
	out.write((char*)&xcs.explorationProbability, sizeof(xcs.explorationProbability));
	out.write((char*)&xcs.doGeneticAlgorithmSubsumption, sizeof(xcs.doGeneticAlgorithmSubsumption));
	out.write((char*)&xcs.doActionSetSubsumption, sizeof(xcs.doActionSetSubsumption));
	size_t size = xcs.population.size();
	out.write((char*)&size, sizeof(size));
	for (Classifier* cl : xcs.population) {
		for (int i = 0; i < 64; i++)
			out.write((char*)&cl->condition[i], sizeof(cl->condition[i]));
		out.write((char*)&cl->action.from, sizeof(cl->action.from));
		out.write((char*)&cl->action.to, sizeof(cl->action.to));
		out.write((char*)&cl->action.promote, sizeof(cl->action.promote));
		out.write((char*)&cl->prediction, sizeof(cl->prediction));
		out.write((char*)&cl->predictionError, sizeof(cl->predictionError));
		out.write((char*)&cl->fitness, sizeof(cl->fitness));
		out.write((char*)&cl->experience, sizeof(cl->experience));
		out.write((char*)&cl->actionSetSize, sizeof(cl->actionSetSize));
		out.write((char*)&cl->timeStamp, sizeof(cl->timeStamp));
		out.write((char*)&cl->numerosity, sizeof(cl->numerosity));
	}
	out.close();
}

XCS loadXCS(std::string filePath, bool learning) {
	std::ifstream in(filePath, std::ifstream::in | std::ifstream::binary);
	XCS xcs;
	int currentTime;
	in.read((char*)&currentTime, sizeof(xcs.currentTime));
	xcs.currentTime = currentTime;
	in.read((char*)&xcs.populationSize, sizeof(xcs.populationSize));
	in.read((char*)&xcs.learningRate, sizeof(xcs.learningRate));
	in.read((char*)&xcs.alpha, sizeof(xcs.alpha));
	in.read((char*)&xcs.errorThreshold, sizeof(xcs.errorThreshold));
	in.read((char*)&xcs.power, sizeof(xcs.power));
	in.read((char*)&xcs.discountFactor, sizeof(xcs.discountFactor));
	in.read((char*)&xcs.geneticAlgorithmThreshold, sizeof(&xcs.geneticAlgorithmThreshold));
	in.read((char*)&xcs.crossoverProbability, sizeof(xcs.crossoverProbability));
	in.read((char*)&xcs.mutationProbability, sizeof(xcs.mutationProbability));
	in.read((char*)&xcs.experienceDeletionThreshold, sizeof(xcs.experienceDeletionThreshold));
	in.read((char*)&xcs.fitnessDeletionThreshold, sizeof(xcs.fitnessDeletionThreshold));
	in.read((char*)&xcs.experienceSubsumptionThreshold, sizeof(xcs.experienceSubsumptionThreshold));
	in.read((char*)&xcs.jokerProbability, sizeof(xcs.jokerProbability));
	in.read((char*)&xcs.initialPrediction, sizeof(xcs.initialPrediction));
	in.read((char*)&xcs.initialPredictionError, sizeof(xcs.initialPredictionError));
	in.read((char*)&xcs.initialFitness, sizeof(xcs.initialFitness));
	in.read((char*)&xcs.explorationProbability, sizeof(xcs.explorationProbability));
	in.read((char*)&xcs.doGeneticAlgorithmSubsumption, sizeof(xcs.doGeneticAlgorithmSubsumption));
	in.read((char*)&xcs.doActionSetSubsumption, sizeof(xcs.doActionSetSubsumption));
	size_t size = 0;
	in.read((char*)&size, sizeof(size));
	for (int i = 0; i < size; i++) {
		Classifier* cl = new Classifier();
		for (int i = 0; i < 64; i++)
			in.read((char*)&cl->condition[i], sizeof(cl->condition[i]));
		in.read((char*)&cl->action.from, sizeof(cl->action.from));
		in.read((char*)&cl->action.to, sizeof(cl->action.to));
		in.read((char*)&cl->action.promote, sizeof(cl->action.promote));
		in.read((char*)&cl->prediction, sizeof(cl->prediction));
		in.read((char*)&cl->predictionError, sizeof(cl->predictionError));
		in.read((char*)&cl->fitness, sizeof(cl->fitness));
		in.read((char*)&cl->experience, sizeof(cl->experience));
		in.read((char*)&cl->actionSetSize, sizeof(cl->actionSetSize));
		in.read((char*)&cl->timeStamp, sizeof(cl->timeStamp));
		in.read((char*)&cl->numerosity, sizeof(cl->numerosity));
		xcs.population.push_back(cl);
	}
	in.close();
	if (!learning) {
		xcs.explorationProbability = 0;
		xcs.learning = false;
	}
	return xcs;
}

bool stopCondition() { return true; }

void prepareTestProcedure(unsigned int moveRuleCounter, unsigned int foldRepetitionCounter) {
	int firstSelection = 0;
	int secondSelection = 0;
	std::shared_ptr<Player> white = nullptr;
	std::shared_ptr<Player> black = nullptr;
	do {
		if (firstSelection > 2) {
			//system("cls");
			std::cout << "Wrong selection, try again" << std::endl;
		}
		std::cout << "Choose the AI for the first player : " << std::endl;
		std::cout << " 0. Negamax" << std::endl;
		std::cout << " 1. QLearning" << std::endl;
		std::cout << " 2. XCS" << std::endl;
		std::cout << "Type the AI's ID : ";
		std::cin >> firstSelection;
		switch (firstSelection)
		{
		case 0: {
			//system("cls");
			unsigned int depth = 0;
			do {
				if (depth == 0 || depth > INT_MAX) {
					//system("cls");
					std::cout << "Wrong selection, try again." << std::endl;
				}
				std::cout << "Type the depth (more than 1 included) : ";
				std::cin >> depth;
			} while (depth == 0 || depth > INT_MAX);
			//system("cls");
			int negamaxMode = 0;
			do {
				if (negamaxMode > 2) {
					//system("cls");
					std::cout << "Wrong selection, try again" << std::endl;
				}
				std::cout << "Choose the negamax mode : " << std::endl;
				std::cout << " 0. Simple negamax." << std::endl;
				std::cout << " 1. MTDF." << std::endl;
				std::cout << " 2. Iterative MTDF." << std::endl;
				std::cout << "Type the mode's ID : ";
				std::cin >> negamaxMode;
			} while (negamaxMode > 2);
			white = std::shared_ptr<Player>(new Negamax(depth, -100000, 100000, INT_MAX, (NegamaxMode)negamaxMode));
			break;
		}
		case 1: {
			//system("cls");
			int fileNumber = 0;
			std::string path = "/Saves/QLearning";
			for (const std::filesystem::directory_entry& entry : std::filesystem::directory_iterator(path)) {
				if (entry.path().string().find("_white.ql"))
					fileNumber++;
			}
			if (fileNumber == 0) {
				std::string enter;
				std::cout << "No QLearning profile file found" << std::endl;
				std::cout << "Type anything to exit : ";
				std::cin >> enter;
			}
			else {
				unsigned int selection = 0;
				do {
					if (selection > fileNumber) {
						//system("cls");
						std::cout << "Wrong selection, try again." << std::endl;
					}
					std::cout << "Choose the QLearning profile : " << std::endl;
					int i = 0;
					for (const std::filesystem::directory_entry& entry : std::filesystem::directory_iterator(path)) {
						if (entry.path().string().find("_white.ql")) {
							std::string fileName = entry.path().filename().string();
							size_t lastIndex = fileName.find_last_of(".");
							std::cout << " " << i << ". " << fileName.substr(0, lastIndex) << std::endl;
							i++;
						}
					}
					std::cout << "Type the profile's ID : ";
					std::cin >> selection;
				} while (selection > fileNumber);
				int i = 0;
				for (const std::filesystem::directory_entry& entry : std::filesystem::directory_iterator(path)) {
					if (entry.path().string().find("_white.ql")) {
						if (selection == i) {
							white = std::shared_ptr<Player>(new QLearning(loadQLearning(entry.path().string())));
						}
						i++;
					}
				}
			}
			break;
		}
		case 2: {
			//system("cls");
			int fileNumber = 0;
			std::string path = "./Saves/XCS";
			for (const std::filesystem::directory_entry& entry : std::filesystem::directory_iterator(path)) {
				if (entry.path().string().find("_white.xcs"))
					fileNumber++;
			}
			if (fileNumber == 0) {
				std::string enter;
				std::cout << "No QLearning profile file found" << std::endl;
				std::cout << "Type anything to exit : ";
				std::cin >> enter;
			}
			else {
				unsigned int selection = 0;
				do {
					if (selection > fileNumber) {
						//system("cls");
						std::cout << "Wrong selection, try again." << std::endl;
					}
					std::cout << "Choose the XCS profile : " << std::endl;
					int i = 0;
					for (const std::filesystem::directory_entry& entry : std::filesystem::directory_iterator(path)) {
						if (entry.path().string().find("_white.xcs")) {
							std::string fileName = entry.path().filename().string();
							size_t lastIndex = fileName.find_last_of(".");
							std::cout << " " << i << ". " << fileName.substr(0, lastIndex) << std::endl;
							i++;
						}
					}
					std::cout << "Type the profile's ID : ";
					std::cin >> selection;
				} while (selection > fileNumber);
				int i = 0;
				for (const std::filesystem::directory_entry& entry : std::filesystem::directory_iterator(path)) {
					if (entry.path().string().find("_white.xcs")) {
						if (selection == i) {
							white = std::shared_ptr<Player>(new XCS(loadXCS(entry.path().string(), false)));
						}
						i++;
					}
				}
			}
			break;
		}
		default:
			break;
		}
	} while (firstSelection > 2 || white == nullptr);
	do {
		if (secondSelection > 2) {
			//system("cls");
			std::cout << "Wrong selection, try again" << std::endl;
		}
		std::cout << "Choose the AI for the second player : " << std::endl;
		std::cout << " 0. Negamax" << std::endl;
		std::cout << " 1. QLearning" << std::endl;
		std::cout << " 2. XCS" << std::endl;
		std::cout << "Type the AI's ID : ";
		std::cin >> secondSelection;
		switch (secondSelection)
		{
		case 0: {
			//system("cls");
			unsigned int depth = 0;
			do {
				if (depth == 0 || depth > INT_MAX) {
					//system("cls");
					std::cout << "Wrong selection, try again." << std::endl;
				}
				std::cout << "Type the depth (more than 1 included) : ";
				std::cin >> depth;
			} while (depth == 0 || depth > INT_MAX);
			//system("cls");
			int negamaxMode = 0;
			do {
				if (negamaxMode > 2) {
					//system("cls");
					std::cout << "Wrong selection, try again" << std::endl;
				}
				std::cout << "Choose the negamax mode : " << std::endl;
				std::cout << " 0. Simple negamax." << std::endl;
				std::cout << " 1. MTDF." << std::endl;
				std::cout << " 2. Iterative MTDF." << std::endl;
				std::cout << "Type the mode's ID : ";
				std::cin >> negamaxMode;
			} while (negamaxMode > 2);
			black = std::shared_ptr<Player>(new Negamax(depth, -100000, 100000, 100000, (NegamaxMode)negamaxMode));
			break;
		}
		case 1: {
			//system("cls");
			int fileNumber = 0;
			std::string path = "/Saves/QLearning";
			for (const std::filesystem::directory_entry& entry : std::filesystem::directory_iterator(path)) {
				if (entry.path().string().find("_black.ql"))
					fileNumber++;
			}
			if (fileNumber == 0) {
				std::string enter;
				std::cout << "No QLearning profile file found" << std::endl;
				std::cout << "Type anything to exit : ";
				std::cin >> enter;
			}
			else {
				unsigned int selection = 0;
				do {
					if (selection > fileNumber) {
						//system("cls");
						std::cout << "Wrong selection, try again." << std::endl;
					}
					std::cout << "Choose the QLearning profile : " << std::endl;
					int i = 0;
					for (const std::filesystem::directory_entry& entry : std::filesystem::directory_iterator(path)) {
						if (entry.path().string().find("_black.ql")) {
							std::string fileName = entry.path().filename().string();
							size_t lastIndex = fileName.find_last_of(".");
							std::cout << " " << i << ". " << fileName.substr(0, lastIndex) << std::endl;
							i++;
						}
					}
					std::cout << "Type the profile's ID : ";
					std::cin >> selection;
				} while (selection > fileNumber);
				int i = 0;
				for (const std::filesystem::directory_entry& entry : std::filesystem::directory_iterator(path)) {
					if (entry.path().string().find("_black.ql")) {
						if (selection == i) {
							black = std::shared_ptr<Player>(new QLearning(loadQLearning(entry.path().string())));
						}
						i++;
					}
				}
			}
			break;
		}
		case 2: {
			//system("cls");
			int fileNumber = 0;
			std::string path = "./Saves/XCS";
			for (const std::filesystem::directory_entry& entry : std::filesystem::directory_iterator(path)) {
				if (entry.path().string().find("_black.xcs"))
					fileNumber++;
			}
			if (fileNumber == 0) {
				std::string enter;
				std::cout << "No XCS profile file found" << std::endl;
				std::cout << "Type anything to exit : ";
				std::cin >> enter;
			}
			else {
				unsigned int selection = 0;
				do {
					if (selection > fileNumber) {
						//system("cls");
						std::cout << "Wrong selection, try again." << std::endl;
					}
					std::cout << "Choose the XCS profile : " << std::endl;
					int i = 0;
					for (const std::filesystem::directory_entry& entry : std::filesystem::directory_iterator(path)) {
						if (entry.path().string().find("_black.xcs")) {
							std::string fileName = entry.path().filename().string();
							size_t lastIndex = fileName.find_last_of(".");
							std::cout << " " << i << ". " << fileName.substr(0, lastIndex) << std::endl;
							i++;
						}
					}
					std::cout << "Type the profile's ID : ";
					std::cin >> selection;
				} while (selection > fileNumber);
				int i = 0;
				for (const std::filesystem::directory_entry& entry : std::filesystem::directory_iterator(path)) {
					if (entry.path().string().find("_black.xcs")) {
						if (selection == i) {
							black = std::shared_ptr<Player>(new XCS(loadXCS(entry.path().string(), false)));
						}
						i++;
					}
				}
			}
			break;
		}
		default:
			break;
		}
	} while (secondSelection > 2 || black == nullptr);
	unsigned int numberGames = 0;
	do {
		if (numberGames > INT_MAX) {
			std::cout << "Wrong selection, try again." << std::endl;
		}
		std::cout << "Type the number of games to accomplish : ";
		std::cin >> numberGames;
	} while (numberGames > INT_MAX);
	bool display;
	std::cout << "Type the display mode (0 = no display, any other value = displaying) : ";
	std::cin >> display;
	runTestProcedure(moveRuleCounter, foldRepetitionCounter,
		white, black,
		numberGames, display);
}

std::array<int, 3> runTestProcedure(unsigned int moveRuleCounter, unsigned int foldRepetitionCounter,
	std::shared_ptr<Player>& white, std::shared_ptr<Player>& black,
	unsigned int numberGames, bool display) {
	std::array<int, 3> outcomes = { 0,0,0 };
	for (unsigned int i = 0; i < numberGames; i++) {
		Game game(white, black, display);
		game.id = i;
		game.moveCounterMax = moveRuleCounter;
		game.foldRepetionMax = foldRepetitionCounter;
		int outcome = game.runGame();
		outcomes[outcome == O_White ? 0 : outcome == O_Black ? 1 : 2]++;
		switch (outcome)
		{
		case O_White:
			std::cout << "white wins";
			break;
		case O_Black:
			std::cout << "black wins";
			break;
		case O_DrawByBlocked:
			std::cout << "draw by king blocked";
			break;
		case O_DrawByNoMove:
			std::cout << "draw by no important move";
			break;
		case O_DrawByRepetion:
			std::cout << "draw by fold repetition";
			break;
		default:
			std::cout << "unknown";
			break;
		}
		std::cout << std::endl;
		i++;
	}
	std::cout << "Outcomes : " << std::endl;
	std::cout << " - White : " << outcomes[0] << std::endl;
	std::cout << " - Black : " << outcomes[1] << std::endl;
	std::cout << " - Draw : " << outcomes[2] << std::endl;
	return outcomes;
}
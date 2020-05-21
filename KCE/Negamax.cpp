#include "Negamax.h"


Negamax::Negamax(int depth, NegamaxMode mode) {
	this->depth = depth;
	useAlphaBetaPruning = false;
	useTranspositionTable = false;
	this->mode = mode;
}

Negamax::Negamax(int depth, int alpha, int beta, NegamaxMode mode) {
	this->depth = depth;
	this->alpha = alpha;
	this->beta = beta;
	useAlphaBetaPruning = true;
	useTranspositionTable = false;
	this->mode = mode;
}

Negamax::Negamax(int depth, int transpositionTableSize, NegamaxMode mode) {
	this->depth = depth;
	transpositionTable = std::unordered_map<NodeKey, NodeInfo>();
	useAlphaBetaPruning = false;
	useTranspositionTable = true;
	this->mode = mode;
}

Negamax::Negamax(int depth, int alpha, int beta, int transpositionTableSize, NegamaxMode mode) {
	this->depth = depth;
	this->alpha = alpha;
	this->beta = beta;
	transpositionTable = std::unordered_map<NodeKey, NodeInfo>();
	useAlphaBetaPruning = true;
	useTranspositionTable = true;
	this->mode = mode;
}

int score(Game& game, Color color) {
	int score = 0;
	for (int i = 2; i < 8; i++) {
		score += tokenScore(game, color, (Token)i);
		score -= tokenScore(game, (Color)(color ^ 1), (Token)i);
	}
	/*
	score -= doubledPawnScore(game, color);
	score += doubledPawnScore(game, (Color)(color ^ 1));
	score -= blockedPawnScore(game, color);
	score += blockedPawnScore(game, (Color)(color ^ 1));
	score -= isolatedPawnScore(game, color);
	score += isolatedPawnScore(game, (Color)(color ^ 1));
	*/
	score += game.legalMoves[color].size();
	score -= game.legalMoves[(Color)(color ^ 1)].size();
	return score;
}

int tokenScore(Game& game, Color color, Token token) {
	int score = 0;
	unsigned __int64 acc = game.board.getColorTokensType(color, token);
	while (acc) {
		score += tokenScoreArray[token - 2];
		acc ^= 1ull << BitScanForward64(acc);
	}
	return score;
}

int doubledPawnScore(Game& game, Color color) {
	int score = 0;
	unsigned __int64 checkColumn = 0x0101010101010101;
	for (int i = 0; i < 8; i++) {
		unsigned __int64 pawnColumn = (checkColumn << i) & game.board.getColorTokensType(color, T_Pawn);
		if (pawnColumn) {
			int pawns = 0;
			while (pawnColumn) {
				pawnColumn ^= 1ull << BitScanForward64(pawnColumn);
				pawns++;
			}
			score += 5 * (pawns - 1);
		}
	}
	return score;
}

int blockedPawnScore(Game& game, Color color) {
	int score = 0;
	unsigned __int64 acc = game.board.getColorTokensType(color, T_Pawn);
	if (color)
		acc >>= 8ull;
	else
		acc <<= 8ull;
	acc &= game.board.getOccupiedCells();
	while (acc) {
		score += 5;
		acc ^= 1ull << BitScanForward64(acc);
	}
	return score;
}

int isolatedPawnScore(Game& game, Color color) {
	int score = 0;
	unsigned __int64 checkColumn = 0x0101010101010101;
	for (int i = 0; i < 8; i++) {
		unsigned __int64 acc = (checkColumn << i) << 1 | (checkColumn << i) >> 1;
		acc &= game.board.getColorTokensType(color, T_Pawn);
		if (!acc) {
			acc = checkColumn & game.board.getColorTokensType(color, T_Pawn);
			while (acc) {
				score += 5;
				acc ^= 1ull << BitScanForward64(acc);
			}
		}
	}
	return score;
}

int Negamax::bestMoves(Game& game, Color color, int depth, int alpha, int beta, std::vector<Move>& moves) {
	int terminalNode;
	int alphaOrigin = alpha;
	size_t isEntryContained = 0;
	NodeInfo tableEntry = { 0,0,0 };
	nodecount++;

	if (useTranspositionTable) {
		if ((isEntryContained = transpositionTable.count({ game.board, color })) && (tableEntry = transpositionTable[{game.board, color}]).depth >= depth) {
			int flag = tableEntry.flag;
			if (flag == NodeInfo::B_Exact) {
				transpositionCutOffCount++;
				return tableEntry.value;
			}
			else if (useAlphaBetaPruning) {
				if (flag == NodeInfo::B_LowerBound)
					alpha = std::max(alpha, tableEntry.value);
				else if (flag == NodeInfo::B_UpperBound)
					beta = std::min(beta, tableEntry.value);
				if (alpha >= beta) {
					transpositionCutOffCount++;
					return tableEntry.value;
				}
			}
		}
	}

	terminalNode = game.endGame((Color)(color ^ 1));
	if (depth == 0 && terminalNode == O_NotDone) {
		depthTerminalNode++;
		return -score(game, (Color)(color ^ 1));
	}

	else if (terminalNode != O_NotDone) {
		if (terminalNode >= O_DrawByBlocked) {
			drawTerminalNode++;
			return 0;
		}
		else {
			color ? whiteTerminalNode++ : blackTerminalNode++;
			return -100000;
		}
	}
	int value = INT_MIN;
	std::vector<Move> trash;
	std::vector<Move> legalMoves;
	std::unordered_map<Move, int> movesScores;
	if (!isEntryContained) {
		legalMoves = game.legalMoves[color];
		for (Move legalMove : legalMoves)
			movesScores.insert({ legalMove, INT_MIN });
	}
	else {
		for (std::pair<Move, int> moveScorePair : tableEntry.moveScorePairs) {
			movesScores.insert(moveScorePair);
			legalMoves.push_back(moveScorePair.first);
		}
	}
	for (Move legalMove : legalMoves) {
		Game copy(game);
		int tokenMoved, tokenCaptured;
		copy.board.execMoveByIndex(legalMove, color, tokenMoved, tokenCaptured);
		copy.moveCounterUpdate(color, tokenMoved, tokenCaptured);
		copy.foldRepetitionUpdate(color);
		copy.legalMoves[color ^ 1] = color ? whiteLegalMoves(copy.board) : blackLegalMoves(copy.board);
		int newValue = -bestMoves(copy, (Color)(color ^ 1), depth - 1, -beta, -alpha, trash);
		movesScores[legalMove] = newValue;
		if (value < newValue) {
			value = newValue;
			moves.clear();
		}
		if (value <= newValue)
			moves.push_back(legalMove);
		if (useAlphaBetaPruning) {
			alpha = std::max(alpha, value);
			if (alpha >= beta) {
				cutOffCount++;
				break;
			}
		}
	}

	if (useTranspositionTable) {
		if (useAlphaBetaPruning) {
			if (value <= alphaOrigin)
				tableEntry = { depth, value, NodeInfo::B_UpperBound };
			else if (value >= beta)
				tableEntry = { depth, value, NodeInfo::B_LowerBound };
			else
				tableEntry = { depth, value, NodeInfo::B_Exact };
		}
		else
			tableEntry = { depth, value, NodeInfo::B_Exact };
		std::vector<std::pair<Move, int>> moveScorePairs;
		for (std::pair<Move, int> moveScore : movesScores)
			moveScorePairs.push_back(moveScore);
		std::sort(moveScorePairs.begin(), moveScorePairs.end(), ScoreSortDescending());
		tableEntry.moveScorePairs = moveScorePairs;
		if (isEntryContained)
			transpositionTable.insert({ {game.board, color}, tableEntry });
		else
			transpositionTable[{game.board, color}] = tableEntry;
	}

	return value;
}

int Negamax::MTDF(Game& game, Color color, int depth, int f, std::vector<Move>& moves) {
	int g = f;
	int upperBound = INT_MAX;
	int lowerBound = INT_MIN;
	while (lowerBound < upperBound) {
		int beta = std::max(g, lowerBound + 1);
		g = bestMoves(game, color, depth, beta - 1, beta, moves);
		if (g < beta)
			upperBound = g;
		else
			lowerBound = g;
	}
	return g;
}

int Negamax::iterativeMTDF(Game& game, Color color, int depth, std::vector<Move>& moves) {
	int firstGuess = 0;
	for (int d = 1; d <= depth; d++) {
		firstGuess = MTDF(game, color, d, firstGuess, moves);
	}
	return firstGuess;
}

Move Negamax::play(Game& game, Color color) {
	nodecount = 0;
	depthTerminalNode = 0;
	drawTerminalNode = 0;
	whiteTerminalNode = 0;
	blackTerminalNode = 0;
	cutOffCount = 0;
	transpositionCutOffCount = 0;
	if (useTranspositionTable)
		transpositionTable.clear();
	std::vector<Move> moves;
	//Game copy(game);
	if (mode == NM_Negamax)
		bestMoves(game, color, depth, alpha, beta, moves);
	else if (mode == NM_MTDF)
		MTDF(game, color, depth, 0, moves);
	else
		iterativeMTDF(game, color, depth, moves);
	if (moves.size() == 0)
		game.endGame((Color)(color ^ 1));
	return moves[rand() % moves.size()];
}

void Negamax::printState() {
	std::cout << "Total node : " << nodecount << std::endl;
	std::cout << "Depth terminal node : " << depthTerminalNode << std::endl;
	std::cout << "Draw terminal node : " << drawTerminalNode << std::endl;
	std::cout << "White terminal node : " << whiteTerminalNode << std::endl;
	std::cout << "Black terminal node : " << blackTerminalNode << std::endl;
	std::cout << "Cut-off count : " << cutOffCount << std::endl;
	std::cout << "Transposition cutoff count : " << transpositionCutOffCount << std::endl;
}

size_t std::hash<NodeKey>::operator()(NodeKey const& nk) const {
	size_t hash = 1;
	std::hash<Board> hashBoard;
	std::hash<int> hashInt;
	hash ^= hashBoard(nk.board) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
	hash ^= hashInt(nk.color) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
	return hash;
}

bool std::equal_to<NodeKey>::operator()(const NodeKey& nk1, const NodeKey& nk2) const {
	std::equal_to<Board> eqBoard;
	return eqBoard(nk1.board, nk2.board) && nk1.color == nk2.color;
}

/*
bool ScoreSortDescending::operator()(const std::pair<int, int>& ii1, const std::pair<int, int>& ii2) const {
	return ii1.second > ii2.second;
}
*/

bool ScoreSortDescending::operator() (const std::pair<Move, int>& mi1, const std::pair<Move, int>& mi2) const {
	return mi1.second > mi2.second;
}
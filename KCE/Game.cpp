#include "Game.h"

Game::Game(std::shared_ptr<Player>& white, std::shared_ptr<Player>& black) {
	players[0] = white;
	players[1] = black;
	currentPlayer = C_White;
	outcome = (Outcome)O_NotDone;
	moveCounter = { 0,0 };
	display = false;
}

Game::Game(Board& board, std::shared_ptr<Player>& white, std::shared_ptr<Player>& black) {
	this->board = Board(board);
	players[0] = white;
	players[1] = black;
	currentPlayer = C_White;
	outcome = (Outcome)O_NotDone;
	moveCounter = { 0,0 };
	display = false;
}

Game::Game(std::shared_ptr<Player>& white, std::shared_ptr<Player>& black, bool display) {
	players[0] = white;
	players[1] = black;
	currentPlayer = C_White;
	outcome = (Outcome)O_NotDone;
	moveCounter = { 0,0 };
	this->display = display;
}

Game::Game(Board& board, std::shared_ptr<Player>& white, std::shared_ptr<Player>& black, bool display) {
	this->board = Board(board);
	players[0] = white;
	players[1] = black;
	currentPlayer = C_White;
	outcome = (Outcome)O_NotDone;
	moveCounter = { 0,0 };
	this->display = display;
}

/*
Game::Game(std::shared_ptr<Player> white, std::shared_ptr<Player> black, bool display) {
	players[0] = white;
	players[1] = black;
	currentPlayer = C_White;
	outcome = (Outcome)O_NotDone;
	moveCounter = { 0,0 };
	this->display = display;
}
*/

Game::Game(const Game& game) {
	board = Board(game.board);
	players[0] = game.players[0];
	players[1] = game.players[1];
	currentPlayer = game.currentPlayer;
	outcome = game.outcome;
	moveCounter = { game.moveCounter[0], game.moveCounter[1] };
	foldRepetition = std::unordered_map<Board, int>(game.foldRepetition);
	jail = { game.jail[0], game.jail[1] };
	for (Move move : game.legalMoves[0])
		legalMoves[0].push_back(move);
	for (Move move : game.legalMoves[1])
		legalMoves[1].push_back(move);
	display = game.display;
}

/*
Game::Game(Player& white, Player& black) {
	//players = new Player* [2];
	players[0] = std::shared_ptr<Player>(&white);
	players[1] = std::shared_ptr<Player>(&black);
	currentPlayer = C_White;
	outcome = (Outcome)O_NotDone;
	moveCounter = { 0,0 };
}

Game::Game(Board& board, Player& white, Player& black) {
	this->board = Board(board);
	//players = new Player* [2];
	players[0] = std::shared_ptr<Player>(&white);
	players[1] = std::shared_ptr<Player>(&black);
	currentPlayer = C_White;
	outcome = (Outcome)O_NotDone;
	moveCounter = { 0,0 };
}

Game::Game(const Game& game) {
	board = Board(game.board);
	//players = new Player* [2];
	players[0] = game.players[0];
	players[1] = game.players[1];
	currentPlayer = game.currentPlayer;
	outcome = game.outcome;
	moveCounter = { game.moveCounter[0], game.moveCounter[1] };
	foldRepetition = std::unordered_map<Board, int>(game.foldRepetition);
	jail = { game.jail[0], game.jail[1] };
}
*/

/*
Game::~Game() {

}
*/

int Game::runGame() {
	if (currentPlayer) {
		legalMoves[C_White] = whiteLegalMoves(board);
		endGame(C_Black);
		legalMoves[C_Black] = blackLegalMoves(board);
		while (outcome == O_NotDone) {
			turnCount++;
			runTurn(C_Black);
			if (outcome == O_NotDone)
				runTurn(C_White);
		}
	}
	else {
		legalMoves[C_Black] = blackLegalMoves(board);
		endGame(C_White);
		legalMoves[C_White] = whiteLegalMoves(board);
		while (outcome == O_NotDone) {
			turnCount++;
			runTurn(C_White);
			if (outcome == O_NotDone)
				runTurn(C_Black);
		}
	}
	players[C_White]->afterEnd(*this, C_White);
	players[C_Black]->afterEnd(*this, C_Black);
	return outcome;
}

void Game::runTurn(Color c) {
	currentPlayer = c;
	Move move = players[c]->play(*this, c);
	int tokenMoved, tokenCaptured;
	if (display) {
		turnPrint(move);
		players[c]->printState();
	}
	board.execMoveByIndex(move, c, tokenMoved, tokenCaptured);
	if (tokenCaptured)
		jail[c].push_back(tokenCaptured);
	moveCounterUpdate(c, tokenMoved, tokenCaptured);
	foldRepetitionUpdate(c);
	if (display)
		board.PrintBoard();
	legalMoves[c ^ 1] = c ? whiteLegalMoves(board) : blackLegalMoves(board);
	outcome = endGame(c);
	players[c ^ 1]->afterOpponentMove(*this, (Color)(c ^ 1), move);
}

/*

void Game::whiteTurn() {
	currentPlayer = C_White;
	legalMoves[C_White] = whiteLegalMoves(board);
	Move move = players[C_White]->play(*this, C_White);
	int tokenMoved, tokenCaptured;
	WhiteTurnPrint(move);
	board.execMoveByIndex(move, C_White, tokenMoved, tokenCaptured);
	if (tokenCaptured)
		jail[C_White].push_back(tokenCaptured);
	moveCounterUpdate(C_White, tokenMoved, tokenCaptured);
	foldRepetitionUpdate(C_White);
	board.PrintBoard();
	outcome = endGame(C_White);
}

void Game::blackTurn() {
	currentPlayer = C_Black;
	legalMoves[C_Black] = blackLegalMoves(board);
	Move move = players[C_White]->play(*this, C_Black);
	int tokenMoved, tokenCaptured;
	BlackTurnPrint(move);
	board.execMoveByIndex(move, C_Black, tokenMoved, tokenCaptured);
	if (tokenCaptured)
		jail[C_Black].push_back(tokenCaptured);
	moveCounterUpdate(C_Black, tokenMoved, tokenCaptured);
	foldRepetitionUpdate(C_Black);
	board.PrintBoard();
	outcome = endGame(C_Black);
}

*/

void Game::moveCounterUpdate(Color c, int tokenMoved, int tokenCaptured) {
	if (tokenMoved % 10 == T_Pawn || tokenCaptured) {
		moveCounter[c] = 0;
		foldRepetition.clear();
	}
	else
		moveCounter[c]++;
}

void Game::foldRepetitionUpdate(Color c) {
	if (foldRepetition.count(board))
		foldRepetition[board]++;
	else
		foldRepetition.insert({ board, 1 });
}

int Game::endGame(Color color) {
	if (!legalMoves[(Color)(color ^ 1)].size()) {
		if (check(board, (Color)(color ^ 1)))
			return color;
		else
			return O_DrawByBlocked;
	}
	else {
		if (moveCounter[0] >= moveCounterMax && moveCounter[1] >= moveCounterMax)
			return O_DrawByNoMove;
		else if (foldRepetition[board] >= foldRepetionMax)
			return O_DrawByRepetion;
	}
	return O_NotDone;
}

/*
void Game::reset() {
	board = new Board();
	currentPlayer = C_White;
	outcome = (Outcome)O_NotDone;
	moveCounter = new int[2]{ 0,0 };
	foldRepetition.clear();
}
*/

void Game::turnPrint(Move move) {
	std::cout << "Game ID : " << id << " - Turn : " << turnCount << std::endl;
	std::string token;
	int j, k;
	for (j = 0; j < 2; j++) {
		if (board.getColorTokens((Color)j) & (1ull << move.from)) {
			for (k = 2; k < 8; k++) {
				if (board.getColorTokensType((Color)j, (Token)k) & (1ull << move.from)) {
					token = colorTokenString[j] + colorTokenString[k];
					break;
				}
			}
			break;
		}
	}
	std::cout << "Move : " << token << " | " << move.from << " -> " << move.to << std::endl;
}

void Game::WhiteTurnPrint(Move move) {
	std::cout << "Gmae ID : " << id << " - Turn : " << turnCount << std::endl;
	std::string token;
	int j, k;
	for (j = 0; j < 2; j++) {
		if (board.getColorTokens((Color)j) & (1ull << move.from)) {
			for (k = 2; k < 8; k++) {
				if (board.getColorTokensType((Color)j, (Token)k) & (1ull << move.from)) {
					token = colorTokenString[j] + colorTokenString[k];
					break;
				}
			}
			break;
		}
	}
	std::cout << "Move : " << token << " | " << move.from << " -> " << move.to << std::endl;
}

void Game::BlackTurnPrint(Move move) {
	std::cout << "Gmae ID : " << id << " - Turn : " << turnCount << std::endl;
	std::string token;
	for (int j = 0; j < 2; j++) {
		if (board.getColorTokens((Color)j) & (1ull << move.from)) {
			for (int k = 2; k < 8; k++) {
				if (board.getColorTokensType((Color)j, (Token)k) & (1ull << move.from)) {
					token = colorTokenString[j] + colorTokenString[k];
					break;
				}
			}
			break;
		}
	}
	std::cout << "Move : " << token << " | " << move.from << " -> " << move.to;
	std::cout << std::endl;
}

/*
Game::Game(Player& white, Player& black) {
	board = new Board();
	players = new Player*[2];
	players[0] = &white;
	players[1] = &black;
	currentPlayer = C_White;
	outcome = (Outcome)O_NotDone;
	moveCounter = new int[2]{ 0,0 };
	jail = new std::vector<int>[2];
}

Game::Game(Board& board, Player& white, Player& black) {
	this->board = new Board(board);
	players = new Player*[2];
	players[0] = &white;
	players[1] = &black;
	currentPlayer = C_White;
	outcome = (Outcome)O_NotDone;
	moveCounter = new int[2]{ 0,0 };
	jail = new std::vector<int>[2];
}

Game::Game(const Game& game) {
	board = new Board(*game.board);
	players = new Player*[2];
	players[0] = game.players[0];
	players[1] = game.players[1];
	currentPlayer = game.currentPlayer;
	outcome = game.outcome;
	moveCounter = new int[2]{ game.moveCounter[0], game.moveCounter[1] };
	foldRepetition = std::unordered_map<Board, int>(game.foldRepetition);
	jail = new std::vector<int>[2]{ game.jail[0], game.jail[1] };
}

Game::~Game() {
	//delete players[0];
	//delete players[1];
	delete[] players;
	delete[] moveCounter;
	delete[] jail;
	delete board;
}

int Game::runGame() {
	if (currentPlayer) {
		endGame(C_White);
		while (outcome == O_NotDone) {
			turnCount++;
			runTurn(C_Black);
			if (outcome == O_NotDone)
				runTurn(C_White);
		}
	}
	else {
		endGame(C_Black);
		while (outcome == O_NotDone) {
			turnCount++;
			runTurn(C_White);
			if (outcome == O_NotDone)
				runTurn(C_Black);
		}
	}
	players[C_White]->afterEnd(*this, C_White);
	players[C_Black]->afterEnd(*this, C_Black);
	return outcome;
}

void Game::runTurn(Color c) {
	currentPlayer = c;
	Move move = players[c]->play(*this, c);
	int tokenMoved, tokenCaptured;
	players[c]->printState();
	turnPrint(move);
	board->execMoveByIndex(move, c, tokenMoved, tokenCaptured);
	if(tokenCaptured)
		jail[c].push_back(tokenCaptured);
	moveCounterUpdate(c, tokenMoved, tokenCaptured);
	foldRepetitionUpdate(c);
	board->PrintBoard();
	outcome = endGame(c);
	players[c ^ 1]->afterOpponentMove(*this, (Color)(c ^ 1), move);
}

void Game::whiteTurn() {
	Move move = players[C_White]->play(*this, C_White);
	int tokenMoved, tokenCaptured;
	WhiteTurnPrint(move);
	board->execMoveByIndex(move, C_White, tokenMoved, tokenCaptured);
	if(tokenCaptured)
		jail[C_White].push_back(tokenCaptured);
	moveCounterUpdate(C_White, tokenMoved, tokenCaptured);
	foldRepetitionUpdate(C_White);
	board->PrintBoard();
	legalMoves = blackLegalMoves(*board);
	outcome = endGame(C_White);
}

void Game::blackTurn() {
	Move move = players[C_White]->play(*this, C_Black);
	int tokenMoved, tokenCaptured;
	BlackTurnPrint(move);
	board->execMoveByIndex(move, C_Black, tokenMoved, tokenCaptured);
	if(tokenCaptured)
		jail[C_Black].push_back(tokenCaptured);
	moveCounterUpdate(C_Black, tokenMoved, tokenCaptured);
	foldRepetitionUpdate(C_Black);
	board->PrintBoard();
	legalMoves = whiteLegalMoves(*board);
	outcome = endGame(C_Black);
}

void Game::moveCounterUpdate(Color c, int tokenMoved, int tokenCaptured) {
	moveCounter[c] = (tokenMoved % 10 == T_Pawn || tokenCaptured) ? 0 : moveCounter[c] + 1;
}

void Game::foldRepetitionUpdate(Color c) {
	if (foldRepetition.count(*board))
		foldRepetition[*board]++;
	else
		foldRepetition.insert({ *board, 1 });
}

int Game::endGame(Color color) {
	legalMoves = color ? whiteLegalMoves(*board) : blackLegalMoves(*board);
	if (!legalMoves.size()) {
		if (check(*board, (Color)(color ^ 1)))
			return color;
		else
			return O_DrawByBlocked;
	}
	else {
		if (moveCounter[0] >= moveCounterMax && moveCounter[1] >= moveCounterMax)
			return O_DrawByNoMove;
		else if (foldRepetition[*board] >= foldRepetionMax)
			return O_DrawByRepetion;
	}
	return O_NotDone;
}

void Game::reset() {
	board =  new Board();
	currentPlayer = C_White;
	outcome = (Outcome)O_NotDone;
	moveCounter = new int[2]{ 0,0 };
	foldRepetition.clear();
}

void Game::turnPrint(Move move) {
	std::cout << "Game ID : " << id << " - Turn : " << turnCount << std::endl;
	std::string token;
	int j, k;
	for (j = 0; j < 2; j++) {
		if (board->getColorTokens((Color)j) & (1ull << move.from)) {
			for (k = 2; k < 8; k++) {
				if (board->getColorTokensType((Color)j, (Token)k) & (1ull << move.from)) {
					token = colorTokenString[j] + colorTokenString[k];
					break;
				}
			}
			break;
		}
	}
	std::cout << "Move : " << token << " | " << move.from << " -> " << move.to << std::endl;
}

void Game::WhiteTurnPrint(Move move) {
	std::cout << "Gmae ID : " << id << " - Turn : " << turnCount << std::endl;
	std::string token;
	int j, k;
	for (j = 0; j < 2; j++) {
		if (board->getColorTokens((Color)j) & (1ull << move.from)) {
			for (k = 2; k < 8; k++) {
				if (board->getColorTokensType((Color)j, (Token)k) & (1ull << move.from)) {
					token = colorTokenString[j] + colorTokenString[k];
					break;
				}
			}
			break;
		}
	}
	std::cout << "Move : " << token << " | " << move.from << " -> " << move.to << std::endl;
}

void Game::BlackTurnPrint(Move move) {
	std::cout << "Gmae ID : " << id << " - Turn : " << turnCount << std::endl;
	std::string token;
	for (int j = 0; j < 2; j++) {
		if (board->getColorTokens((Color)j) & (1ull << move.from)) {
			for (int k = 2; k < 8; k++) {
				if (board->getColorTokensType((Color)j, (Token)k) & (1ull << move.from)) {
					token = colorTokenString[j] + colorTokenString[k];
					break;
				}
			}
			break;
		}
	}
	std::cout << "Move : " << token << " | " << move.from << " -> " << move.to;
	std::cout << std::endl;
}
*/
#pragma once
#include "Move.h"
#include "Player.h"
#include <unordered_map>

/*
 * Enum�ration d�crivant les possibilit�s de fin de partie.
 */
enum Outcome {
	O_White,
	O_Black,
	O_NotDone,
	O_DrawByBlocked,
	O_DrawByRepetion,
	O_DrawByNoMove,
};

class Player;

/*
 * Structure repr�sentant une partie d'�chec.
 */
class Game {
public:
	// Identifiant d'une partie.
	int id = 0;
	// Compteur de tours.
	int turnCount = 0;
	// Plateau de jeu li� � cette partie.
	Board board;
	// Variable contenant les pi�ces captur�es.
	std::array<std::vector<int>, 2> jail;
	// Variable contenant les structures virtuelles des 2 joueurs.
	std::array<std::shared_ptr<Player>, 2> players;
	// Identifiant du joueur actuel.
	Color currentPlayer;
	// Identifiant de la fin de la partie.
	int outcome;
	// Variable contenant les mouvements l�gaux des 2 joueurs.
	std::array <std::vector<Move>, 2> legalMoves;
	//std::array<bool, 2> castlings;
	// Seuil de la r�gle des mouvements signifiants.
	int moveCounterMax = 50;
	// Compteur de la r�gle des mouvements signifiants.
	std::array<int, 2> moveCounter;
	// Seuil de la r�gle des r�p�titions de position.
	int foldRepetionMax = 3;
	// Compteur de la r�gle des r�p�titions de position.
	std::unordered_map<Board, int> foldRepetition;
	// Variable bool�nne d�terminant l'affichage.
	bool display;

	Game(std::shared_ptr<Player>& white, std::shared_ptr<Player>& black);
	Game(Board& board, std::shared_ptr<Player>& white, std::shared_ptr<Player>& black);
	Game(std::shared_ptr<Player>& white, std::shared_ptr<Player>& black, bool display);
	Game(Board& board, std::shared_ptr<Player>& white, std::shared_ptr<Player>& black, bool display);
	Game(const Game& game);

	// Fonction d'ex�cution de la partie, renvoyant l'issue de la partie.
	int runGame();
	// Fonction d'ex�cution d'un tour pour une certaine couleur.
	void runTurn(Color c);
	// Fonction de mise-�-jour du compteur de la r�gle des mouvements signifiants.
	void moveCounterUpdate(Color c, int tokenMoved, int tokenCaptured);
	// Fonction de mise-�-jour du compteur de la r�gle des r�p�tions de position.
	void foldRepetitionUpdate(Color c);
	// Fonction de calcul de l'issue d'une partie.
	int endGame(Color oppColor);

	// Fonction d'affichage d'un tour.
	void turnPrint(Move move);
	void WhiteTurnPrint(Move move);
	void BlackTurnPrint(Move move);
};

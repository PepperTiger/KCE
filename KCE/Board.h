#pragma once
#ifndef BOARD_H
#define BOARD_H
#include "Util.h"

/*
 * Enum�ration utilis� pour d�crire une couleur.
 */
enum Color {
	C_White,
	C_Black
};

/*
 * Enum�ration utilis� pour d�cire une pi�ce.
 */
enum Token
{
	T_Pawn = 2,
	T_Knight,
	T_Bishop,
	T_Rook,
	T_Queen,
	T_King
};

/*
 * Stucture utilis� pour d�crire un mouvement.
 */
struct Move {
	// Index de d�part du mouvement.
	int from;
	// Index d'arriv�e du mouvement.
	int to;
	// ID de la promotion de la pi�ce.
	int promote;
	Move();
	Move(int from, int to, int promote);
	Move(const Move& move);
};

/*
 * Structure utilis� pour d�crire le plateau de jeu.
 */
class Board {
public:
	// Tableau repr�sntant la position des pi�ces selon leur type ou couleur (voir rapport).
	std::array<unsigned __int64, 8> board;
	Board();
	Board(std::array<unsigned __int64, 8> board);
	Board(const Board& board);

	// R�cup�re la position des cases occup�es du plateau.
	unsigned __int64 getOccupiedCells();
	// R�cup�re la position des cases vides du plateau.
	unsigned __int64 getEmptyCells();
	// R�cup�re la position des pi�ces d'une certaine couleur du plateau.
	unsigned __int64 getColorTokens(Color c);
	// R�cup�re la position des pi�ces d'un certain type du plateau.
	unsigned __int64 getTokenType(Token t);
	// R�cup�re la position des pi�ces d'un certain type et d'une certaine couleur.
	unsigned __int64 getColorTokensType(Color c, Token t);
	// R�cup�re la position des pi�ces d'une certaine couleur sans le roi.
	unsigned __int64 getColorTokensWithoutKing(Color c);
	// Fonctions d'ex�cutions des mouvements.
	void execMoveByIndex(int from, int to, Color c, Token t);
	void execMoveByIndex(Move move, Color c, int& tokenMoved, int& tokenCaptured);
	// Fonction d'impression du plateu.
	void PrintBoard() const;
};

// Tableau de transpostion entre l'�num�ration des pi�ces et leur repr�sentation en caract�res.
const std::string colorTokenString[8] = { "W", "B", "P", "N", "B", "R", "Q", "K" };

namespace std {
	// Fonction de hachage des plateaux.
	template<> struct hash<Board> {
		std::size_t operator()(Board const& b) const;
	};
	// Fonction d'�galit� des plateaux.
	template<> struct equal_to<Board> {
		bool operator()(const Board& b1, const Board& b2) const;
	};

	// Fonction de hachage des mouvements.
	template<> struct hash<Move> {
		std::size_t operator()(Move const& b) const;
	};
	// Fonction d'�galit� des mouvements.
	template<> struct equal_to<Move> {
		bool operator()(const Move& m1, const Move& m2) const;
	};
}

#endif // HEADERS
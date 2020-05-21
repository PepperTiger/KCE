#pragma once
#ifndef BOARD_H
#define BOARD_H
#include "Util.h"

/*
 * Enumération utilisé pour décrire une couleur.
 */
enum Color {
	C_White,
	C_Black
};

/*
 * Enumération utilisé pour décire une pièce.
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
 * Stucture utilisé pour décrire un mouvement.
 */
struct Move {
	// Index de départ du mouvement.
	int from;
	// Index d'arrivée du mouvement.
	int to;
	// ID de la promotion de la pièce.
	int promote;
	Move();
	Move(int from, int to, int promote);
	Move(const Move& move);
};

/*
 * Structure utilisé pour décrire le plateau de jeu.
 */
class Board {
public:
	// Tableau représntant la position des pièces selon leur type ou couleur (voir rapport).
	std::array<unsigned __int64, 8> board;
	Board();
	Board(std::array<unsigned __int64, 8> board);
	Board(const Board& board);

	// Récupère la position des cases occupées du plateau.
	unsigned __int64 getOccupiedCells();
	// Récupère la position des cases vides du plateau.
	unsigned __int64 getEmptyCells();
	// Récupère la position des pièces d'une certaine couleur du plateau.
	unsigned __int64 getColorTokens(Color c);
	// Récupère la position des pièces d'un certain type du plateau.
	unsigned __int64 getTokenType(Token t);
	// Récupère la position des pièces d'un certain type et d'une certaine couleur.
	unsigned __int64 getColorTokensType(Color c, Token t);
	// Récupère la position des pièces d'une certaine couleur sans le roi.
	unsigned __int64 getColorTokensWithoutKing(Color c);
	// Fonctions d'exécutions des mouvements.
	void execMoveByIndex(int from, int to, Color c, Token t);
	void execMoveByIndex(Move move, Color c, int& tokenMoved, int& tokenCaptured);
	// Fonction d'impression du plateu.
	void PrintBoard() const;
};

// Tableau de transpostion entre l'énumération des pièces et leur représentation en caractères.
const std::string colorTokenString[8] = { "W", "B", "P", "N", "B", "R", "Q", "K" };

namespace std {
	// Fonction de hachage des plateaux.
	template<> struct hash<Board> {
		std::size_t operator()(Board const& b) const;
	};
	// Fonction d'égalité des plateaux.
	template<> struct equal_to<Board> {
		bool operator()(const Board& b1, const Board& b2) const;
	};

	// Fonction de hachage des mouvements.
	template<> struct hash<Move> {
		std::size_t operator()(Move const& b) const;
	};
	// Fonction d'égalité des mouvements.
	template<> struct equal_to<Move> {
		bool operator()(const Move& m1, const Move& m2) const;
	};
}

#endif // HEADERS
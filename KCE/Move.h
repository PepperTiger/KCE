#pragma once
#ifndef MOVE_H
#define MOVE_H
#include "Board.h"

// Constante de plateau vide.
const unsigned __int64 emptySet = 0;
// Constante de plateau plein.
const unsigned __int64 fullSet = 0xffffffffffffffff;
// Constante indiquant le plateau sans la colonne est.
const unsigned __int64 notEastColumn = 0x7f7f7f7f7f7f7f7f;
// Constante indiquant le plateau sans la colonne ouest.
const unsigned __int64 notWestColumn = 0xfefefefefefefefe;
// Constante indiquant le plateau sans les 2 colonnes est.
const unsigned __int64 not2EastFile = 0x3f3f3f3f3f3f3f3f;
// Constante indiquant le plateau sans les 2 colonnes ouest.
const unsigned __int64 not2WestFile = 0xfcfcfcfcfcfcfcfc;
// Constante indiquant la ligne de commencement des pions blancs.
const unsigned __int64 whitePawnRow = 0x000000000000ff00;
// Constante indiquant la ligne de commencement des pions noirs.
const unsigned __int64 blackPawnRow = 0x00ff000000000000;
//const unsigned __int64** inBetweenArray; //= initInBetweenArray();

// Fonction retournant les positions au nord des positions donn�es en argument.
unsigned __int64 north(unsigned __int64 tokenMask);
// Fonction retournant les positions au sud des positions donn�es en argument.
unsigned __int64 south(unsigned __int64 tokenMask);
// Fonction retournant les positions � l'est des positions donn�es en argument.
unsigned __int64 east(unsigned __int64 tokenMask);
// Fonction retournant les positions � l'ouest des positions donn�es en argument.
unsigned __int64 west(unsigned __int64 tokenMask);

// Fonction retournant les positions au nord-est des positions donn�es en argument.
unsigned __int64 northEast(unsigned __int64 tokenMask);
// Fonction retournant les positions au nord-ouest des positions donn�es en argument.
unsigned __int64 northWest(unsigned __int64 tokenMask);
// Fonction retournant les positions au sud-est des positions donn�es en argument.
unsigned __int64 southEast(unsigned __int64 tokenMask);
// Fonction retournant les positions au sud-ouest des positions donn�es en argument.
unsigned __int64 southWest(unsigned __int64 tokenMask);

// Fonction retournant le mouvement th�orique d'une case des positions des pions blancs donn�es en argument.
unsigned __int64 whitePawnMoves(unsigned __int64 whitePawns);
// Fonction retournant le mouvement th�orique de 2 cases des positions des pions blancs donn�es en argument.
unsigned __int64 whitePawn2Moves(unsigned __int64 whitePawns);
// Fonction retournant le mouvement th�orique de 2 cases de l'index de la position du pions blancs donn�es en argument.
unsigned __int64 whitePawn2Moves(unsigned __int64 occupancy, int index);
// Fonction retournant l'attaque nord-est th�orique des positions des pions blancs donn�es en argument.
unsigned __int64 whitePawnEastAttacks(unsigned __int64 whitePawns);
// Fonction retournant l'attaque nord-ouest th�orique des positions des pions blancs donn�es en argument.
unsigned __int64 whitePawnWestAttacks(unsigned __int64 whitePawns);
// Fonction retournant les attaques th�oriques des positions des pions blancs donn�es en argument.
unsigned __int64 whitePawnAllAttacks(unsigned __int64 whitePawns);

// Fonction retournant le mouvement th�orique d'une case des positions des pions noirs donn�es en argument.
unsigned __int64 blackPawnMoves(unsigned __int64 blackPawns);
// Fonction retournant le mouvement th�orique de 2 cases des positions des pions noirs donn�es en argument.
unsigned __int64 blackPawn2Moves(unsigned __int64 blackPawns);
// Fonction retournant le mouvement th�orique de 2 cases de l'index de la position du pions noirs donn�es en argument.
unsigned __int64 blackPawn2Moves(unsigned __int64 occupancy, int index);
// Fonction retournant l'attaque sud-est th�orique des positions des pions noirs donn�es en argument.
unsigned __int64 blackPawnEastAttacks(unsigned __int64 blackPawns);
// Fonction retournant l'attaque sud-est th�orique des positions des pions noirs donn�es en argument.
unsigned __int64 blackPawnWestAttacks(unsigned __int64 blackPawns);
// Fonction retournant les attaques th�oriques des positions des pions noirs donn�es en argument.
unsigned __int64 blackPawnAllAttacks(unsigned __int64 blackPawns);

// Fonction retournant les mouvements th�oriques des positions des rois donn�es en argument.
unsigned __int64 kingAttacks(unsigned __int64 kingSet);

// Fonction retournant les positions au nord-nord-est des positions donn�es en argument.
unsigned __int64 northNorthEast(unsigned __int64 tokenMask);
// Fonction retournant les positions au nord-est-est des positions donn�es en argument.
unsigned __int64 northEastEast(unsigned __int64 tokenMask);
// Fonction retournant les positions au nord-nord-ouest des positions donn�es en argument.
unsigned __int64 northNorthWest(unsigned __int64 tokenMask);
// Fonction retournant les positions au nord-ouest-ouest des positions donn�es en argument.
unsigned __int64 northWestWest(unsigned __int64 tokenMask);
// Fonction retournant les positions au sud-sud-est des positions donn�es en argument.
unsigned __int64 southSouthEast(unsigned __int64 tokenMask);
// Fonction retournant les positions au sud-est-est des positions donn�es en argument.
unsigned __int64 southEastEast(unsigned __int64 tokenMask);
// Fonction retournant les positions au sud-sud-ouest des positions donn�es en argument.
unsigned __int64 southSouthWest(unsigned __int64 tokenMask);
// Fonction retournant les positions au sud-ouest-ouest des positions donn�es en argument.
unsigned __int64 southWestWest(unsigned __int64 tokenMask);

// Fonction retournant les mouvements th�oriques des positions des cavaliers donn�es en argument.
unsigned __int64 knightAttacks(unsigned __int64 knights);

// Fonction retournant la ligne contenant l'index de la position donn�.
unsigned __int64 rowMask(int index);
// Fonction retournant la ligne contenant l'index de la position donn� except� l'index lui m�me.
unsigned __int64 rowMaskEx(int index);
// Fonction retournant la colonne contenant l'index de la position donn�.
unsigned __int64 columnMask(int index);
// Fonction retournant la colonne contenant l'index de la position donn� except� l'index lui m�me.
unsigned __int64 columnMaskEx(int index);
// Fonction retournant la diagonale contenant l'index de la position donn�.
unsigned __int64 diagonalMask(int index);
// Fonction retournant la diagonale contenant l'index de la position donn� except� l'index lui m�me.
unsigned __int64 diagonalMaskEx(int index);
// Fonction retournant l'anti-diagonale contenant l'index de la position donn�.
unsigned __int64 antidiagonalMask(int index);
// Fonction retournant l'anti-diagonale contenant l'index de la position donn� except� l'index lui m�me.
unsigned __int64 antidiagonalMaskEx(int index);
// Fonction retournant les mouvements th�oriques d'une tour sur un plateau vide � partir de l'index de la position donn�.
unsigned __int64 rookMask(int index);
// Fonction retournant les mouvements th�oriques d'une tour sur un plateau vide � partir de l'index de la position donn� except� l'index lui m�me.
unsigned __int64 rookMaskEx(int index);
// Fonction retournant les mouvements th�oriques d'un fou sur un plateau vide � partir de l'index de la position donn�.
unsigned __int64 bishopMask(int index);
// Fonction retournant les mouvements th�oriques d'un fou sur un plateau vide � partir de l'index de la position donn� except� l'index lui m�me.
unsigned __int64 bishopMaskEx(int index);
// Fonction retournant les mouvements th�oriques d'une reine sur un plateau vide � partir de l'index de la position donn�.
unsigned __int64 queenMask(int index);
// Fonction retournant les mouvements th�oriques d'une reine sur un plateau vide � partir de l'index de la position donn� except� l'index lui m�me.
unsigned __int64 queenMaskEx(int index);

// Fonction retournant les mouvements horrizontaux glissants th�oriques � partir de l'index donn�.
unsigned __int64 rowAttacks(unsigned __int64 occupancy, int index);
//unsigned __int64 rowAttacks2(unsigned __int64 occupancy, int mask);
// Fonction retournant les mouvements diagonaux glissants th�oriques � partir de l'index donn�.
unsigned __int64 diagonalAttacks(unsigned __int64 occupancy, int index);
// Fonction retournant les mouvements anti-diagonaux glissants th�oriques � partir de l'index donn�.
unsigned __int64 antidiagonalAttacks(unsigned __int64 occupancy, int index);
// Fonction retournant les mouvements verticaux glissants th�oriques � partir de l'index donn�.
unsigned __int64 columnAttacks(unsigned __int64 occupancy, int index);

// Fonction retournant les mouvements th�oriques de la tour � l'index donn�.
unsigned __int64 rookAttacks(unsigned __int64 occupancy, int index);
// Fonction retournant les mouvements th�oriques du fou � l'index donn�.
unsigned __int64 bishopAttacks(unsigned __int64 occupancy, int index);
// Fonction retournant les mouvements th�oriques de la reine � l'index donn�.
unsigned __int64 queenAttacks(unsigned __int64 occupancy, int index);

unsigned __int64 xrayRookAttacks(unsigned __int64 occupancy, unsigned __int64 blockers, int index);

unsigned __int64 xrayBishopAttacks(unsigned __int64 occupancy, unsigned __int64 blockers, int index);

unsigned __int64 xrayQueenAttacks(unsigned __int64 occupancy, unsigned __int64 blockers, int index);

// Fonction retournant tous les mouvements pseudo-l�gaux d'une couleur donn� � partir d'un plateau donn�.
unsigned __int64 getAllAttacks(Board& board, Color color);
// Fonction retournant une valeur non-nulle si le roi de la couleur donn� est en �chec. 
unsigned __int64 check(Board& board, Color color);

// Fonction retournant les mouvements pseudo-l�gaux � partir de l'index d'un pion blanc donn�.
unsigned __int64 whitePawnPseudoLegalMoves(Board& board, int index);
// Fonction retournant les mouvements pseudo-l�gaux � partir de l'index d'un pion noir donn�.
unsigned __int64 blackPawnPseudoLegalMoves(Board& board, int index);
// Fonction retournant les mouvements pseudo-l�gaux � partir de l'index d'une tour d'une couleur donn�.
unsigned __int64 rookPseudoLegalMoves(Board& board, int index, Color color);
// Fonction retournant les mouvements pseudo-l�gaux � partir de l'index d'un fou d'une couleur donn�.
unsigned __int64 bishopPseudoLegalMoves(Board& board, int index, Color color);
// Fonction retournant les mouvements pseudo-l�gaux � partir de l'index d'un cavalier d'une couleur donn�.
unsigned __int64 knightPseudoLegalMoves(Board& board, int index, Color color);
// Fonction retournant les mouvements pseudo-l�gaux � partir de l'index d'une reine d'une couleur donn�.
unsigned __int64 queenPseudoLegalMoves(Board& board, int index, Color color);
// Fonction retournant les mouvements pseudo-l�gaux � partir de l'index d'un roi d'une couleur donn�.
unsigned __int64 kingPseudoLegalMoves(Board& board, int index, Color color);

unsigned __int64 tokenLegalMovesMask(Board& board, int index, unsigned __int64 pseudoMoves, Color color, Token token);

// Fonction retournant une liste des mouvements l�gaux d'une pi�ce donn�.
std::vector<Move> tokenLegalMovesVector(Board& board, int index, unsigned __int64 pseudoMoves, Color color, Token token);
// Fonction retournant une liste des mouvements l�gaux d'un pion blanc donn�.
std::vector<Move> whitePawnLegalMovesVector(Board& board, int index);
// Fonction retournant une liste des mouvements l�gaux d'un pion noir donn�.
std::vector<Move> blackPawnLegalMovesVector(Board& board, int index);

// Fontion retournant une liste des mouvements blancs.
std::vector<Move> whiteLegalMoves(Board& board);
// Fontion retournant une liste des mouvements noirs.
std::vector<Move> blackLegalMoves(Board& board);

std::vector<Move> mergeMoves(std::vector<Move> v1, std::vector<Move> v2);

#endif // !HEADERS

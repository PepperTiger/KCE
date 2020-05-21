#include "Player.h"

Move Player::play(Game& game, Color color) {
	return game.legalMoves[color][rand() % game.legalMoves.size()];
};

void Player::printState() {};

void Player::afterEnd(Game& game, Color) {};

void Player::afterOpponentMove(Game& game, Color color, Move move) {};
#include "Board.h"

Board::Board() {
	board[T_Pawn] = 0x00ff00000000ff00;
	board[T_Rook] = 0x8100000000000081;
	board[T_Knight] = 0x4200000000000042;
	board[T_Bishop] = 0x2400000000000024;
	board[T_Queen] = 0x0800000000000008;
	board[T_King] = 0x1000000000000010;
	board[C_White] = 0x000000000000ffff;
	board[C_Black] = 0xffff000000000000;
}
Board::Board(std::array<unsigned __int64, 8> board) {
	for (int i = 0; i < 8; i++)
		this->board[i] = board[i];
}

Board::Board(const Board& board) {
	for (int i = 0; i < 8; i++)
		this->board[i] = board.board[i];
}

Move::Move() : from(0), to(0), promote(0) {};

Move::Move(int from, int to, int promote) : from(from), to(to), promote(promote) {};

Move::Move(const Move& move) {
	from = move.from;
	to = move.to;
	promote = move.promote;
}

unsigned __int64 Board::getOccupiedCells() { return board[C_White] | board[C_Black]; }
unsigned __int64 Board::getEmptyCells() { return ~(board[C_White] | board[C_Black]); }
unsigned __int64 Board::getColorTokens(Color c) { return board[c]; }
unsigned __int64 Board::getTokenType(Token t) { return board[t]; }
unsigned __int64 Board::getColorTokensType(Color c, Token t) { return board[c] & board[t]; }
unsigned __int64 Board::getColorTokensWithoutKing(Color c) { return board[c] & ~board[T_King]; }

void Board::execMoveByIndex(int from, int to, Color c, Token t) {
	for (int i = 0; i < 8; i++)
		board[i] &= ~(1ull << to);
	board[c] ^= (1ull << from) | (1ull << to);
	board[t] ^= (1ull << from) | (1ull << to);
}

void Board::execMoveByIndex(Move move, Color c, int& tokenMoved, int& tokenCaptured) {
	int tokenType = 0; unsigned __int64 tokenMask = 0;
	int i = 1;
	while (!tokenMask) {
		i++;
		tokenMask |= (1ull << move.from) & board[i];
	}
	tokenType = i;
	tokenCaptured = 0;
	for (i = 0; i < 8; i++) {
		if (board[i] & (1ull << move.to))
			tokenCaptured = (c ^ 1) * 10 + i;
		board[i] &= ~(1ull << move.to);
	}
	board[c] ^= (1ull << move.from) | (1ull << move.to);
	board[tokenType] ^= (1ull << move.from);
	board[move.promote] ^= (1ull << move.to);
	tokenMoved = c * 10 + tokenType;
}

void Board::PrintBoard() const {
	int board[64];
	for (int i = 0; i < 64; i++) {
		board[i] = 0;
		for (int j = 0; j < 2; j++) {
			if (this->board[j] & (1ull << i)) {
				for (int k = 2; k < 8; k++) {
					if (this->board[k] & (1ull << i)) {
						board[i] = j * 10 + k;
						break;
					}
				}
				break;
			}
		}
	}
	for (int i = 7; i >= 0; i--) {
		std::cout << "|";
		for (int j = 0; j < 8; j++) {
			if (!board[8 * i + j])
				std::cout << "  ";
			else
				std::cout << colorTokenString[board[8 * i + j] / 10] << colorTokenString[board[8 * i + j] % 10];
			std::cout << "|";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

size_t std::hash<Board>::operator()(Board const& b) const {
	size_t hash = 1;
	std::hash<unsigned __int64> hashU64;
	for (int i = 0; i < 8; i++)
		hash ^= hashU64(b.board[i]) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
	return hash;
}

bool std::equal_to<Board>::operator()(const Board& b1, const Board& b2) const {
	for (int i = 0; i < 8; i++) {
		if (b1.board[i] != b2.board[i])
			return false;
	}
	return true;
}

size_t std::hash<Move>::operator()(Move const& m) const {
	size_t hash = 1;
	std::hash<int> hashInt;
	hash ^= hashInt(m.from) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
	hash ^= hashInt(m.to) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
	hash ^= hashInt(m.promote) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
	return hash;
}

bool std::equal_to<Move>::operator()(const Move& m1, const Move& m2) const {
	return m1.from == m2.from && m1.to == m2.to && m1.promote == m2.promote;
}

#include "Move.h"

unsigned __int64 north(unsigned __int64 tokenMask) { return tokenMask << 8; }
unsigned __int64 south(unsigned __int64 tokenMask) { return tokenMask >> 8; }
unsigned __int64 east(unsigned __int64 tokenMask) { return (tokenMask << 1) & notWestColumn; }
unsigned __int64 west(unsigned __int64 tokenMask) { return (tokenMask >> 1)& notEastColumn; }

unsigned __int64 northEast(unsigned __int64 tokenMask) { return (tokenMask << 9) & notWestColumn; }
unsigned __int64 northWest(unsigned __int64 tokenMask) { return (tokenMask << 7) & notEastColumn; }
unsigned __int64 southEast(unsigned __int64 tokenMask) { return (tokenMask >> 7)& notWestColumn; }
unsigned __int64 southWest(unsigned __int64 tokenMask) { return (tokenMask >> 9)& notEastColumn; }

unsigned __int64 whitePawnMoves(unsigned __int64 whitePawns) { return north(whitePawns); }
unsigned __int64 whitePawn2Moves(unsigned __int64 whitePawns) { return (whitePawns & whitePawnRow) << 16; }
unsigned __int64 whitePawn2Moves(unsigned __int64 occupancy, int index) { return ((((1ull << index) & whitePawnRow) << 8ull) & ~occupancy) << 8ull; }
unsigned __int64 whitePawnEastAttacks(unsigned __int64 whitePawns) { return northEast(whitePawns); }
unsigned __int64 whitePawnWestAttacks(unsigned __int64 whitePawns) { return northWest(whitePawns); }
unsigned __int64 whitePawnAllAttacks(unsigned __int64 whitePawns) { return whitePawnEastAttacks(whitePawns) | whitePawnWestAttacks(whitePawns); }

unsigned __int64 blackPawnMoves(unsigned __int64 blackPawns) { return south(blackPawns); }
unsigned __int64 blackPawn2Moves(unsigned __int64 blackPawns) { return (blackPawns & blackPawnRow) >> 16; }
unsigned __int64 blackPawn2Moves(unsigned __int64 occupancy, int index) { return ((((1ull << index) & blackPawnRow) >> 8ull) & ~occupancy) >> 8ull;; }
unsigned __int64 blackPawnEastAttacks(unsigned __int64 blackPawns) { return southEast(blackPawns); }
unsigned __int64 blackPawnWestAttacks(unsigned __int64 blackPawns) { return southWest(blackPawns); }
unsigned __int64 blackPawnAllAttacks(unsigned __int64 blackPawns) { return blackPawnEastAttacks(blackPawns) | blackPawnWestAttacks(blackPawns); }

unsigned __int64 kingAttacks(unsigned __int64 kingSet) {
	unsigned __int64 attacks = east(kingSet) | west(kingSet);
	kingSet |= attacks;
	attacks |= north(kingSet) | south(kingSet);
	return attacks;
}

unsigned __int64 northNorthEast(unsigned __int64 tokenMask) { return (tokenMask << 17) & notWestColumn; }
unsigned __int64 northEastEast(unsigned __int64 tokenMask) { return (tokenMask << 10) & not2WestFile; }
unsigned __int64 northNorthWest(unsigned __int64 tokenMask) { return (tokenMask << 15) & notEastColumn; }
unsigned __int64 northWestWest(unsigned __int64 tokenMask) { return (tokenMask << 6) & not2EastFile; }

unsigned __int64 southSouthEast(unsigned __int64 tokenMask) { return (tokenMask >> 15)& notWestColumn; }
unsigned __int64 southEastEast(unsigned __int64 tokenMask) { return (tokenMask >> 6)& not2WestFile; }
unsigned __int64 southSouthWest(unsigned __int64 tokenMask) { return (tokenMask >> 17)& notEastColumn; }
unsigned __int64 southWestWest(unsigned __int64 tokenMask) { return (tokenMask >> 10)& not2EastFile; }

unsigned __int64 knightAttacks(unsigned __int64 knights) {
	return northNorthEast(knights) | northEastEast(knights) |
		northNorthWest(knights) | northWestWest(knights) |
		southSouthEast(knights) | southEastEast(knights) |
		southSouthWest(knights) | southSouthWest(knights);
}

unsigned __int64 rowMask(int index) { return 0x00000000000000ffull << (index & 56); }
unsigned __int64 rowMaskEx(int index) { return (1ull << index) ^ rowMask(index); }

unsigned __int64 columnMask(int index) { return 0x0101010101010101ull << (index & 7); }
unsigned __int64 columnMaskEx(int index) { return (1ull << index) ^ columnMask(index); }

unsigned __int64 diagonalMask(int index) {
	const unsigned __int64 maindia = 0x8040201008040201;
	int diag = 8 * (index & 7) - (index & 56);
	int north = -diag & (diag >> 31);
	int south = diag & (-diag >> 31);
	return (maindia >> south) << north;
}
unsigned __int64 diagonalMaskEx(int index) { return (1ull << index) ^ diagonalMask(index); }

unsigned __int64 antidiagonalMask(int index) {
	const unsigned __int64 maindia = 0x0102040810204080;
	int diag = 56 - 8 * (index & 7) - (index & 56);
	int north = -diag & (diag >> 31);
	int south = diag & (-diag >> 31);
	return (maindia >> south) << north;
}
unsigned __int64 antidiagonalMaskEx(int index) { return (1ull << index) ^ antidiagonalMask(index); }

unsigned __int64 rookMask(int index) { return rowMask(index) | columnMask(index); }
unsigned __int64 rookMaskEx(int index) { return rowMaskEx(index) | columnMaskEx(index); }

unsigned __int64 bishopMask(int index) { return diagonalMask(index) | antidiagonalMask(index); }
unsigned __int64 bishopMaskEx(int index) { return diagonalMaskEx(index) | antidiagonalMaskEx(index); }

unsigned __int64 queenMask(int index) { return rookMask(index) | bishopMask(index); }
unsigned __int64 queenMaskEx(int index) { return rookMaskEx(index) | bishopMaskEx(index); }

unsigned __int64 rowAttacks(unsigned __int64 occupancy, int index) {
	unsigned __int64 forward = occupancy ^ (occupancy - 2ull * (1ull << index));
	unsigned __int64 reverse = reverseU64(reverseU64(occupancy) ^ (reverseU64(occupancy) - 2ull * reverseU64(1ull << index)));
	return (forward ^ reverse) & rowMaskEx(index);
}

unsigned __int64 diagonalAttacks(unsigned __int64 occupancy, int index) {
	unsigned __int64 forward = occupancy & diagonalMaskEx(index);
	unsigned __int64 reverse = _byteswap_uint64(forward);
	forward -= 1ull << index;
	reverse -= _byteswap_uint64(1ull << index);
	forward ^= _byteswap_uint64(reverse);
	forward &= diagonalMaskEx(index);
	return forward;
}

unsigned __int64 antidiagonalAttacks(unsigned __int64 occupancy, int index) {
	unsigned __int64 forward = occupancy & antidiagonalMaskEx(index);
	unsigned __int64 reverse = _byteswap_uint64(forward);
	forward -= 1ull << index;
	reverse -= _byteswap_uint64(1ull << index);
	forward ^= _byteswap_uint64(reverse);
	forward &= antidiagonalMaskEx(index);
	return forward;
}

unsigned __int64 columnAttacks(unsigned __int64 occupancy, int index) {
	unsigned __int64 forward = occupancy & columnMaskEx(index);
	unsigned __int64 reverse = _byteswap_uint64(forward);
	forward -= 1ull << index;
	reverse -= _byteswap_uint64(1ull << index);
	forward ^= _byteswap_uint64(reverse);
	forward &= columnMaskEx(index);
	return forward;
}

unsigned __int64 rookAttacks(unsigned __int64 occupancy, int index) { return rowAttacks(occupancy, index) | columnAttacks(occupancy, index); }
unsigned __int64 bishopAttacks(unsigned __int64 occupancy, int index) { return diagonalAttacks(occupancy, index) | antidiagonalAttacks(occupancy, index); }
unsigned __int64 queenAttacks(unsigned __int64 occupancy, int index) { return rookAttacks(occupancy, index) | bishopAttacks(occupancy, index); }

unsigned __int64 xrayRookAttacks(unsigned __int64 occupancy, unsigned __int64 blockers, int index) {
	unsigned __int64 attacks = rookAttacks(occupancy, index);
	blockers &= attacks;
	return attacks ^ rookAttacks(occupancy ^ blockers, index);
}

unsigned __int64 xrayBishopAttacks(unsigned __int64 occupancy, unsigned __int64 blockers, int index) {
	unsigned __int64 attacks = bishopAttacks(occupancy, index);
	blockers &= attacks;
	return attacks ^ rookAttacks(occupancy ^ blockers, index);
}

unsigned __int64 xrayQueenAttacks(unsigned __int64 occupancy, unsigned __int64 blockers, int index) {
	return xrayRookAttacks(occupancy, blockers, index) | xrayBishopAttacks(occupancy, blockers, index);
}

unsigned __int64 getAllAttacks(Board& board, Color color) {
	unsigned __int64 attacks = 0;
	unsigned __int64 occupancy = board.getOccupiedCells();
	attacks |= kingAttacks(board.getColorTokensType(color, T_King));
	unsigned __int64 queens = board.getColorTokensType(color, T_Queen);
	int index;
	while (queens) {
		index = BitScanForward64(queens);
		attacks |= queenAttacks(occupancy, index);
		queens ^= 1ull << index;
	}
	unsigned __int64 bishops = board.getColorTokensType(color, T_Bishop);
	while (bishops) {
		index = BitScanForward64(bishops);
		attacks |= bishopAttacks(occupancy, index);
		bishops ^= 1ull << index;
	}
	unsigned __int64 rooks = board.getColorTokensType(color, T_Rook);
	while (rooks) {
		index = BitScanForward64(rooks);
		attacks |= rookAttacks(occupancy, index);
		rooks ^= 1ull << index;
	}
	unsigned __int64 knights = board.getColorTokensType(color, T_Knight);
	attacks |= knightAttacks(knights);
	unsigned __int64 pawns = board.getColorTokensType(color, T_Pawn);
	attacks |= color ? blackPawnAllAttacks(pawns) : whitePawnAllAttacks(pawns);
	return attacks;
}



unsigned __int64 check(Board& board, Color color) {
	return board.getColorTokensType(color, T_King) & getAllAttacks(board, (Color)(color ^ 1));
}

unsigned __int64 whitePawnPseudoLegalMoves(Board& board, int index) {
	return (whitePawn2Moves(board.getOccupiedCells(), index) | whitePawnMoves(1ull << index)) & board.getEmptyCells() | whitePawnAllAttacks(1ull << index) & board.getColorTokensWithoutKing(C_Black);
}
unsigned __int64 blackPawnPseudoLegalMoves(Board& board, int index) {
	return (blackPawn2Moves(board.getOccupiedCells(), index) | blackPawnMoves(1ull << index)) & board.getEmptyCells() | blackPawnAllAttacks(1ull << index) & board.getColorTokensWithoutKing(C_White);
}
unsigned __int64 rookPseudoLegalMoves(Board& board, int index, Color color) {
	return rookAttacks(board.getOccupiedCells(), index) & (board.getColorTokensWithoutKing((Color)(color ^ 1)) | board.getEmptyCells());
}
unsigned __int64 bishopPseudoLegalMoves(Board& board, int index, Color color) {
	return bishopAttacks(board.getOccupiedCells(), index) & (board.getColorTokensWithoutKing((Color)(color ^ 1)) | board.getEmptyCells());
}
unsigned __int64 knightPseudoLegalMoves(Board& board, int index, Color color) {
	return knightAttacks(1ull << index) & (board.getColorTokensWithoutKing((Color)(color ^ 1)) | board.getEmptyCells());
}
unsigned __int64 queenPseudoLegalMoves(Board& board, int index, Color color) {
	return queenAttacks(board.getOccupiedCells(), index) & (board.getColorTokensWithoutKing((Color)(color ^ 1)) | board.getEmptyCells());
}
unsigned __int64 kingPseudoLegalMoves(Board& board, int index, Color color) {
	return kingAttacks(1ull << index) & (board.getColorTokensWithoutKing((Color)(color ^ 1)) | board.getEmptyCells());
}

unsigned __int64 tokenLegalMovesMask(Board& board, int index, unsigned __int64 pseudoMoves, Color color, Token token) {
	unsigned __int64 legalMoves = 0;
	while (pseudoMoves) {
		int to = BitScanForward64(pseudoMoves);
		Board boardCopy(board);
		boardCopy.execMoveByIndex(index, to, color, token);
		if (!check(boardCopy, color))
			legalMoves |= 1ull << to;
	}
	return legalMoves;
}

std::vector<Move> tokenLegalMovesVector(Board& board, int index, unsigned __int64 pseudoMoves, Color color, Token token) {
	std::vector<Move> legalMoves;
	while (pseudoMoves) {
		int to = BitScanForward64(pseudoMoves);
		Board boardCopy(board);
		boardCopy.execMoveByIndex(index, to, color, token);
		if (!check(boardCopy, color))
			legalMoves.push_back(Move(index, to, token));
		pseudoMoves ^= 1ull << to;
	}
	return legalMoves;
}

std::vector<Move> whitePawnLegalMovesVector(Board& board, int index) {
	std::vector<Move> legalMoves;
	unsigned __int64 pseudoMoves = whitePawnPseudoLegalMoves(board, index);
	while (pseudoMoves) {
		int to = BitScanForward64(pseudoMoves);
		Board boardCopy(board);
		boardCopy.execMoveByIndex(index, to, C_White, T_Pawn);
		if (!check(boardCopy, C_White)) {
			if (!((1ull << index) & blackPawnRow))
				legalMoves.push_back({ index, to, T_Pawn });
			else {
				legalMoves.push_back({ index, to, T_Queen });
				legalMoves.push_back({ index, to, T_Bishop });
				legalMoves.push_back({ index, to, T_Rook });
				legalMoves.push_back({ index, to, T_Knight });
			}
		}
		pseudoMoves ^= 1ull << to;
	}
	return legalMoves;
}

std::vector<Move> blackPawnLegalMovesVector(Board& board, int index) {
	std::vector<Move> legalMoves;
	unsigned __int64 pseudoMoves = blackPawnPseudoLegalMoves(board, index);
	while (pseudoMoves) {
		int to = BitScanForward64(pseudoMoves);
		Board boardCopy(board);
		boardCopy.execMoveByIndex(index, to, C_Black, T_Pawn);
		if (!check(boardCopy, C_Black)) {
			if (!((1ull << index) & whitePawnRow))
				legalMoves.push_back({ index, to, T_Pawn });
			else {
				legalMoves.push_back({ index, to, T_Queen });
				legalMoves.push_back({ index, to, T_Bishop });
				legalMoves.push_back({ index, to, T_Rook });
				legalMoves.push_back({ index, to, T_Knight });
			}
		}
		pseudoMoves ^= 1ull << to;
	}
	return legalMoves;
}

std::vector<Move> whiteLegalMoves(Board& board) {
	std::vector<Move> legalMoves;
	std::vector<Move> tokenLegalMoves;
	unsigned long from;
	unsigned __int64 tokens;

	tokens = board.getColorTokensType(C_White, T_King);
	while (tokens) {
		from = BitScanForward64(tokens);
		tokenLegalMoves = tokenLegalMovesVector(board, from, kingPseudoLegalMoves(board, from, C_White), C_White, T_King);
		legalMoves = mergeMoves(legalMoves, tokenLegalMoves);
		tokens ^= 1ull << from;
	}

	tokens = board.getColorTokensType(C_White, T_Pawn);
	while (tokens) {
		from = BitScanForward64(tokens);
		tokenLegalMoves = whitePawnLegalMovesVector(board, from);
		legalMoves = mergeMoves(legalMoves, tokenLegalMoves);
		tokens ^= 1ull << from;
	}

	tokens = board.getColorTokensType(C_White, T_Rook);
	while (tokens) {
		from = BitScanForward64(tokens);
		tokenLegalMoves = tokenLegalMovesVector(board, from, rookPseudoLegalMoves(board, from, C_White), C_White, T_Rook);
		legalMoves = mergeMoves(legalMoves, tokenLegalMoves);
		tokens ^= 1ull << from;
	}

	tokens = board.getColorTokensType(C_White, T_Bishop);
	while (tokens) {
		from = BitScanForward64(tokens);
		tokenLegalMoves = tokenLegalMovesVector(board, from, bishopPseudoLegalMoves(board, from, C_White), C_White, T_Bishop);
		legalMoves = mergeMoves(legalMoves, tokenLegalMoves);
		tokens ^= 1ull << from;
	}

	tokens = board.getColorTokensType(C_White, T_Knight);
	while (tokens) {
		from = BitScanForward64(tokens);
		tokenLegalMoves = tokenLegalMovesVector(board, from, knightPseudoLegalMoves(board, from, C_White), C_White, T_Knight);
		legalMoves = mergeMoves(legalMoves, tokenLegalMoves);
		tokens ^= 1ull << from;
	}

	tokens = board.getColorTokensType(C_White, T_Queen);
	while (tokens) {
		from = BitScanForward64(tokens);
		tokenLegalMoves = tokenLegalMovesVector(board, from, queenPseudoLegalMoves(board, from, C_White), C_White, T_Queen);
		legalMoves = mergeMoves(legalMoves, tokenLegalMoves);
		tokens ^= 1ull << from;
	}

	return legalMoves;
}

std::vector<Move> blackLegalMoves(Board& board) {
	std::vector<Move> legalMoves;
	std::vector<Move> tokenLegalMoves;
	unsigned long from;
	unsigned __int64 tokens;

	tokens = board.getColorTokensType(C_Black, T_King);
	while (tokens) {
		from = BitScanForward64(tokens);
		tokenLegalMoves = tokenLegalMovesVector(board, from, kingPseudoLegalMoves(board, from, C_Black), C_Black, T_King);
		legalMoves = mergeMoves(legalMoves, tokenLegalMoves);
		tokens ^= 1ull << from;
	}

	tokens = board.getColorTokensType(C_Black, T_Pawn);
	while (tokens) {
		from = BitScanForward64(tokens);
		tokenLegalMoves = blackPawnLegalMovesVector(board, from);
		legalMoves = mergeMoves(legalMoves, tokenLegalMoves);
		tokens ^= 1ull << from;
	}

	tokens = board.getColorTokensType(C_Black, T_Rook);
	while (tokens) {
		from = BitScanForward64(tokens);
		tokenLegalMoves = tokenLegalMovesVector(board, from, rookPseudoLegalMoves(board, from, C_Black), C_Black, T_Rook);
		legalMoves = mergeMoves(legalMoves, tokenLegalMoves);
		tokens ^= 1ull << from;
	}

	tokens = board.getColorTokensType(C_Black, T_Bishop);
	while (tokens) {
		from = BitScanForward64(tokens);
		tokenLegalMoves = tokenLegalMovesVector(board, from, bishopPseudoLegalMoves(board, from, C_Black), C_Black, T_Bishop);
		legalMoves = mergeMoves(legalMoves, tokenLegalMoves);
		tokens ^= 1ull << from;
	}

	tokens = board.getColorTokensType(C_Black, T_Knight);
	while (tokens) {
		from = BitScanForward64(tokens);
		tokenLegalMoves = tokenLegalMovesVector(board, from, knightPseudoLegalMoves(board, from, C_Black), C_Black, T_Knight);
		legalMoves = mergeMoves(legalMoves, tokenLegalMoves);
		tokens ^= 1ull << from;
	}

	tokens = board.getColorTokensType(C_Black, T_Queen);
	while (tokens) {
		from = BitScanForward64(tokens);
		tokenLegalMoves = tokenLegalMovesVector(board, from, queenPseudoLegalMoves(board, from, C_Black), C_Black, T_Queen);
		legalMoves = mergeMoves(legalMoves, tokenLegalMoves);
		tokens ^= 1ull << from;
	}

	return legalMoves;
}

std::vector<Move> mergeMoves(std::vector<Move> v1, std::vector<Move> v2) {
	std::vector<Move> mergedVector;
	mergedVector.reserve(v1.size() + v2.size());
	mergedVector.insert(mergedVector.end(), v1.begin(), v1.end());
	mergedVector.insert(mergedVector.end(), v2.begin(), v2.end());
	return mergedVector;
}

/*
unsigned __int64 isAttacking(unsigned __int64 attacks, unsigned __int64 mask) { return attacks & mask; }
unsigned __int64 isAttacking(unsigned __int64 attacks, int index) { return attacks & (1ull << index); }
unsigned __int64 attacksToKing(Board& board, int kingIndex, Color kingColor) {
	unsigned __int64 opPawns, opKnights, opRQ, opBQ;
	opPawns = board.getColorTokensType((Color)(kingColor & 1), T_Pawn);
	opKnights = board.getColorTokensType((Color)(kingColor & 1), T_Knight);
	opRQ = opBQ = board.getColorTokensType((Color)(kingColor & 1), T_Queen);
	opRQ |= board.getColorTokensType((Color)(kingColor & 1), T_Rook);
	opBQ |= board.getColorTokensType((Color)(kingColor & 1), T_Bishop);
	return isAttacking((kingColor ? blackPawnAllAttacks(opPawns) : whitePawnAllAttacks(opPawns)), kingIndex)
		| isAttacking(knightAttacks(opKnights), kingIndex)
		| isAttacking(rookAttacks(board.getOccupiedCells(), opRQ), kingIndex)
		| isAttacking(bishopAttacks(board.getOccupiedCells(), opRQ), kingIndex);
}

unsigned __int64 obstructed(int from, int to, unsigned __int64 occupancy) { return inBetweenArray[from][to] & occupancy; }
unsigned __int64 getAbsolutePins(Board& board, Color color) {
	unsigned __int64 pinned = 0, opRQ, opBQ;
	unsigned __int64 occupancy = board.getOccupiedCells();
	unsigned __int64 ownTokens = board.getColorTokens(color);
	unsigned long kingIndex; _BitScanForward64(&kingIndex, board.getColorTokensType(color, T_King));
	opRQ = opBQ = board.getColorTokensType((Color)(color & 1), T_Queen);
	opRQ |= board.getColorTokensType((Color)(color & 1), T_Rook);
	unsigned __int64 pinner = xrayRookAttacks(occupancy, ownTokens, kingIndex) & opRQ;
	while (pinner) {
		unsigned long sq;
		_BitScanForward64(&sq, pinner);
		pinned |= obstructed(sq, kingIndex, occupancy) & ownTokens;
		pinner &= pinner - 1;
	}
	opBQ |= board.getColorTokensType((Color)(color & 1), T_Bishop);
	pinner = xrayBishopAttacks(occupancy, ownTokens, kingIndex) & opBQ;
	while (pinner) {
		unsigned long sq;
		_BitScanForward64(&sq, pinner);
		pinned |= obstructed(sq, kingIndex, occupancy) & ownTokens;
		pinner &= pinner - 1;
	}
}
*/

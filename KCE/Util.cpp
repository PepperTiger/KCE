#include "Util.h"

std::vector<char> u64ToVector(unsigned __int64 bitboard) {
	std::vector<char> toReturn;
	while (bitboard) {
		unsigned long index;
		_BitScanForward64(&index, bitboard);
		toReturn.push_back(index);
		bitboard &= bitboard - 1;
	}
	return toReturn;
}

unsigned long BitScanForward64(unsigned __int64 u64) {
	unsigned long index = -1;
	_BitScanForward64(&index, u64);
	return index;
}

void printU64(unsigned __int64 u64) {
	int u64Rep[64];
	for (int i = 0; i < 64; i++)
		u64Rep[i] = 0;
	int x;
	while (u64) {
		x = BitScanForward64(u64);
		u64Rep[x] = 1;
		u64 ^= 1ull << x;
	}
	for (int i = 7; i >= 0; i--) {
		std::cout << "|";
		for (int j = 0; j < 8; j++) {
			if (u64Rep[8 * i + j])
				std::cout << "XX";
			else
				std::cout << "  ";
			std::cout << "|";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

unsigned __int64 reverseU64(unsigned __int64 u64) {
	const unsigned __int64  h1 = 0x5555555555555555;
	const unsigned __int64  h2 = 0x3333333333333333;
	const unsigned __int64  h4 = 0x0F0F0F0F0F0F0F0F;
	const unsigned __int64  v1 = 0x00FF00FF00FF00FF;
	const unsigned __int64  v2 = 0x0000FFFF0000FFFF;
	u64 = ((u64 >> 1ull)& h1) | ((u64 & h1) << 1ull);
	u64 = ((u64 >> 2ull)& h2) | ((u64 & h2) << 2ull);
	u64 = ((u64 >> 4ull)& h4) | ((u64 & h4) << 4ull);
	u64 = ((u64 >> 8ull)& v1) | ((u64 & v1) << 8ull);
	u64 = ((u64 >> 16ull)& v2) | ((u64 & v2) << 16ull);
	u64 = (u64 >> 32ull) | (u64 << 32ull);
	return u64;
}

template<class T>
void hash_combine(std::size_t& seed, const T& v) {
	std::hash<T> hasher;
	seed ^= hasher(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}

double randDouble() {
	return (double)rand() / RAND_MAX;
}

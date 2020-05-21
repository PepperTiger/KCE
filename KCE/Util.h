#pragma once
#ifndef UTIL
#define UTIL
#include <iostream>
#include <vector>
#include <array>
#include <time.h>

class Board;

// Retourne une liste des index des bits activ�s d'un entie de 64 bits.
std::vector<char> u64ToVector(unsigned __int64 bitboard);

// Scanne un entier en partant du bit le moins signifiant et renvoie la valeur de l'index du premier bit activ�, -1 sinon.
unsigned long BitScanForward64(unsigned __int64 u64);

// Affiche les bits activ�s d'un entier de 64 bits.
void printU64(unsigned __int64 u64);

// Renvoie l'entier invers� bit par bit de l'entier � 64 bits donn� en argument.
unsigned __int64 reverseU64(unsigned __int64 u64);

template<class T>
void hash_combine(std::size_t& seed, const T& v);

// Renvoie un nombre al�atoire comprise entre 0 et 1.
double randDouble();

#endif
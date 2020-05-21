#pragma once
#include "Player.h"
#include <algorithm>

// Structure servant de clé pour la table de transposition.
struct NodeKey {
	Board board;
	Color color;
};

namespace std {
	// Fonction de hachage pour NodeKey.
	template<> struct hash<NodeKey> {
		std::size_t operator()(NodeKey const& nk) const;
	};
	// Fonction d'égalite pour NodeKey.
	template<> struct equal_to<NodeKey> {
		bool operator()(const NodeKey& nk1, const NodeKey& nk2) const;
	};
}

struct NodeInfo {
	// Profondeur enregistée.
	int depth;
	// Valeur enregistrée.
	int value;
	// Type de la valeur enregistrée.
	int flag;
	// Score enregistré pour chaque mouvement possible de l'état.
	std::vector<std::pair<Move, int>> moveScorePairs;

	// Enumération décriant les types de valeurs enregistables.
	enum Bound {
		B_Exact,
		B_LowerBound,
		B_UpperBound
	};
};

// Enumération décrivant les modes de Negamax possible.
enum NegamaxMode {
	NM_Negamax,
	NM_MTDF,
	NM_IterativeMTDF
};

class Negamax : public Player {
public:
	// Profondeur d'exploration.
	int depth = 1;
	// Condition d'utilisation de l'élagage alpha-beta.
	bool useAlphaBetaPruning = false;
	// Valeur limite basse.
	int alpha = 0;
	// Valeur limite haute.
	int beta = 0;

	// Condition d'utilisation de la table de transposition.
	bool useTranspositionTable = false;
	// Table de transpostion.
	std::unordered_map<NodeKey, NodeInfo> transpositionTable;

	// Mode de Negamax utilisé.
	NegamaxMode mode;

	Negamax(int depth, NegamaxMode mode);
	Negamax(int depth, int alpha, int beta, NegamaxMode mode);
	Negamax(int depth, int transpositionTableSize, NegamaxMode mode);
	Negamax(int depth, int alpha, int beta, int transpositionTableSize, NegamaxMode mode);
	// Renvoie les meilleurs mouvements et les scores de ces mouvements.
	int bestMoves(Game& game, Color color, int depth, int alpha, int beta, std::vector<Move>& moves);
	// Renvoie les meilleurs mouvements et les scores de ces mouvements.
	int MTDF(Game& game, Color color, int depth, int f, std::vector<Move>& moves);
	// Renvoie les meilleurs mouvements et les scores de ces mouvements.
	int iterativeMTDF(Game& game, Color color, int depth, std::vector<Move>& moves);

	// Fonctions héritées de Player
	Move play(Game& game, Color c) override;
	void printState() override;

private:
	// Compteur de noeud.
	int nodecount = 0;
	// Compteur de noeuds terminaux par profondeur.
	int depthTerminalNode = 0;
	// Compteur de noeuds terminaux par nul.
	int drawTerminalNode = 0;
	// Compteur de noeuds terminaux par victoire des blancs.
	int whiteTerminalNode = 0;
	// Compteur de noeuds terminaux par victoire des noirs.
	int blackTerminalNode = 0;
	// Compteur d'élagage alpha-beta.
	int cutOffCount = 0;
	// Compteur d'élégage par correspondance dans la table de transposition.
	int transpositionCutOffCount = 0;
};

// Constantes des scores de chaque type de pièce.
const int tokenScoreArray[6] = { 10, 30, 40, 50, 90, 2000 };
// Fonction retournant le score d'un état.
int score(Game& game, Color color);
// Fonction retournant le score des pièces d'un type donné.
int tokenScore(Game& game, Color color, Token token);
// Fonction retournant le score des pions doublés.
int doubledPawnScore(Game& game, Color color);
// Fonction retournant le score des pions bloqués.
int blockedPawnScore(Game& game, Color color);
// Fonction retournant le score des pions isolés.
int isolatedPawnScore(Game& game, Color color);

struct ScoreSortDescending {
	bool operator() (const std::pair<Move, int>& mi1, const std::pair<Move, int>& mi2) const;
};


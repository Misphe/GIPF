#pragma once
#include <iostream>
#include <vector>
#include <unordered_set>
#include "Chain.h"
using std::vector;
using std::pair;
using std::unordered_set;

class Gipf;
struct Move;

class GipfAI {
private:
	Gipf* game;

public:
	GipfAI(Gipf& game);

	void printAllPossibilities();
	void printUniqueMovesNumber();
	void printUniqueMovesNumberExt();
	void printPossibleMovesExt();

	Gipf getWinningMove(unordered_set<Gipf>& games);
	unordered_set<Gipf> getAllPossibilities();
	vector<Gipf> makeMove(pair<int, int>& pushSource, pair<int, int>& field);

	// here is stos error
	vector<Move> getAllPossibleMoveCommands();
	
	static void deleteDuplicates(vector<Gipf>& games);
};


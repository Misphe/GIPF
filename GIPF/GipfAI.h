#pragma once
#include <iostream>
#include <vector>
#include "Chain.h"
using std::vector;
using std::pair;

class Gipf;

class GipfAI {
private:
	Gipf* game;

public:
	GipfAI(Gipf& game);

	void printAllPossibilities();
	void printUniqueMovesNumber();

	vector<Gipf> getAllPossibilities();
	Gipf makeMove(pair<int, int>& pushSource, pair<int, int>& field);

	// here is stos error
	vector<Chain> getAllPossibleMoveCommands();
	
	static void deleteDuplicates(vector<Gipf>& games);
};


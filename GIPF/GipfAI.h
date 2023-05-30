#pragma once
#include <iostream>
#include <vector>
using std::vector;
using std::pair;

class Gipf;

class GipfAI {
private:
	Gipf* game;

public:
	GipfAI(Gipf& game);

	void printAllPossibilities();
	vector<Gipf> getAllPossibilities();
	Gipf makeMove(pair<int, int>& pushSource, pair<int, int>& field);

	vector<pair<pair<int, int>, pair<int, int>>> getAllPossibleMoveCommands();
};


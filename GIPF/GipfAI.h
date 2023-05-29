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
	vector<vector<vector<char>>> getAllPossibilities();
	vector<vector<char>> makeMove(pair<int, int>& pushSource, pair<int, int>& field);
};


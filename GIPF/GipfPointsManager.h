#pragma once
#include <iostream>
#include <vector>
#include <set>
#include "Chain.h"

class Gipf;
using std::set;
using std::vector;

class GipfPointsManager {
private:
	Gipf* game;
public:
	GipfPointsManager(Gipf& game);

	void checkChains(std::pair<int, int>& pushVector,std::string start_s, std::string end_s);
	set<Chain> checkChains(int x, int y, std::pair<int, int>& pushVector, bool movedLine);
	Chain checkVertically(int col, int row);
	Chain checkHorizontally(int col, int row);
	Chain checkDiagonally(int col, int row);
	set<Chain> checkMovedLine(int col, int row, std::pair<int, int>& pushVector);

	bool chainCommandValid(char turn, std::string& start_s, std::string end_s);

	std::pair<int, int> findChainEnd(int col, int row, int direction);
	void deleteChain(const std::pair<int, int>& start, const std::pair<int, int>& end, char symbol);
	void deleteChains(set<Chain>& chains, vector<vector<Chain>>& intersectingChains, int x, int y,
		std::pair<int, int>& pushVector, bool movedLine);

	void deleteAdjacent(const std::pair<int, int>& start, std::pair<int, int>& dir, char symbol);
	void deleteMiddle(const std::pair<int, int>& start, const std::pair<int, int>& end, std::pair<int, int>& dir, char symbol);

	int getDirection(std::pair<int, int>& direction);
	static int directionToDimension(int direction);
	std::pair<int, int> getPushVector(std::string& start_s, std::string& end_s);
	std::pair<int, int> getPushVector(const std::pair<int, int>& start, const std::pair<int, int>& end);
};


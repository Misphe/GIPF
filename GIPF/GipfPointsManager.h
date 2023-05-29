#pragma once
#include <iostream>

class Gipf;
struct Chain;

class GipfPointsManager {
private:
	Gipf* game;
public:
	GipfPointsManager(Gipf& game);

	void checkChains(std::pair<int, int>& pushVector,std::string start_s, std::string end_s);
	void checkChains(int x, int y, std::pair<int, int>& pushVector, bool movedLine);
	void checkVertically(int col, int row);
	void checkHorizontally(int col, int row);
	void checkDiagonally(int col, int row);
	void checkMovedLine(int col, int row, std::pair<int, int>& pushVector);

	bool chainCommandValid(char turn, std::string& start_s, std::string end_s);

	std::pair<int, int> findChainEnd(int col, int row, int direction);
	void deleteChain(std::pair<int, int>& start, std::pair<int, int>& end, char symbol);

	void deleteAdjacent(std::pair<int, int>& start, std::pair<int, int>& dir, char symbol);
	void deleteMiddle(std::pair<int, int>& start, std::pair<int, int>& end, std::pair<int, int>& dir, char symbol);

	int getDirection(std::pair<int, int>& direction);
	static int directionToDimension(int direction);
	std::pair<int, int> getPushVector(std::string& start_s, std::string& end_s);
	std::pair<int, int> getPushVector(std::pair<int, int>& start, std::pair<int, int>& end);
};


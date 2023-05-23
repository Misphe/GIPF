#pragma once
#include "Game.h"
#include <iostream>
#include <memory>
#include <vector>
#include "Defines.h"
#include "GipfPlayer.h"

using std::unique_ptr;
using std::vector;

class Gipf : public Game {
private:
	vector<vector<char>> board;
	int size;
	int turn;
	int pawnsCollect;

	GipfPlayer white;
	GipfPlayer black;

public:
	Gipf(vector<vector<char>> board, int size, int pawnsCollect, int whiteMaxPawns, int blackMaxPawns,
		int whitePawns, int blackPawns, int turn);
	void operator=(Gipf& new_gipf);

	void print() override;
	void executeCommand(int command) override;
	int translateCommand(std::string command) override;

	char currentColor();

	static vector<vector<char>> createBoard(int size);

	void loadGameBoard();
	bool loadBoardState();

	void putPawn(int x, int y);
	void endTurn();
	int checkIfWrongIndex(std::pair<int, int>& source_p, std::pair<int, int>& field_p);
	int checkIfUnknownDirection(std::pair<int, int>& source_p, std::pair<int, int>& field_p);
	int moveValid(std::string& source, std::string& field);

	void doMove();
	static void loadMoveSegments(std::string& move, std::string& start, std::string& dir);
	static std::pair<std::string, int> separateIndex(std::string& index);
	std::pair<int, int> getPushVector(std::string& pushSource, std::string& field);
	std::pair<int, int> getWhereToPut(std::string& index);
	bool pushLine(std::string& pushSource, std::string& field, std::pair<int, int>& coords);
};


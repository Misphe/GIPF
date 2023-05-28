#pragma once
#include "Game.h"
#include <iostream>
#include <memory>
#include <vector>
#include "Defines.h"
#include "GipfPlayer.h"
#include "GipfPointsManager.h"

using std::unique_ptr;
using std::vector;

class Gipf : public Game {
private:
	vector<vector<char>> board;
	int size;
	int turn;
	int pawnsCollect;
	std::string state;

	GipfPointsManager manager;
	friend class GipfPointsManager;

	GipfPlayer white;
	GipfPlayer black;

public:
	Gipf(vector<vector<char>> board, int size, int pawnsCollect, int whiteMaxPawns, int blackMaxPawns,
		int whitePawns, int blackPawns, int turn);
	void operator=(Gipf& new_gipf);

	std::pair<int, int> countChainsOnBoard();

	void print() override;
	void executeCommand(int command) override;
	int translateCommand(std::string command) override;

	char currentColor();
	GipfPlayer* currentPlayer();

	int getPawnsCollect();

	static vector<vector<char>> createBoard(int size);

	void loadGameBoard();
	bool loadBoardState();

	void putPawn(int x, int y, std::pair<int, int>& pushVector, bool movedLine);
	void endTurn();

	void doMove();
	static void loadMoveSegments(std::string& move, std::string& start, std::string& dir);
	static std::pair<std::string, int> separateIndex(std::string& index);

	bool pushLine(std::string& pushSource, std::string& field, std::pair<int, int>& coords);
	std::pair<int, int> getWhereToPut(std::string& index);
	std::pair<int, int> getPushVector(std::string& pushSource, std::string& field);

	int moveValid(std::string& source, std::string& field);
	int checkIfWrongIndex(std::pair<int, int>& source_p, std::pair<int, int>& field_p);
	int checkIfUnknownDirection(std::pair<int, int>& source_p, std::pair<int, int>& field_p);
	int checkIfWrongStartingField(std::pair<int, int>& source_p);
	int checkIfWrongDestinationField(std::pair<int, int>& field_p);
	void setBadMoveState(std::string& pushSource, std::string& field);

	void printGameState();
	std::string getGameState();

	static bool insideBoard(vector<vector<char>>& checkedBoard, std::pair<int, int> coords);
	static bool insideBoard(vector<vector<char>>& checkedBoard, int col, int row);
	int countRowsInColumn(int index);
	int countRowStart(int index);
	
	std::pair<int, int> getChainsHorizontally();
	std::pair<int, int> getChainsVertically();
	std::pair<int, int> getChainsDiagonally();

	void handleChainsCountingAtLoading(int& col, int& row, int& whiteCount, int& blackCount,
		int& totalWhiteRows, int& totalBlackRows, int& mode);
};


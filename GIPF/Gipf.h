#pragma once
#include "Game2D.h"
#include <iostream>
#include <memory>
#include <vector>
#include "Defines.h"
#include "GipfPlayer.h"
#include "GipfPointsManager.h"
#include "GipfAI.h"

using std::unique_ptr;
using std::vector;

class Gipf : public Game2D {
private:
	vector<vector<char>> board;
	int size;
	int turn;
	int pawnsCollect;
	bool boardEmpty;
	std::pair<bool, std::string> badMove;
	std::string state;

	GipfPointsManager manager;
	friend class GipfPointsManager;

	GipfPlayer white;
	GipfPlayer black;

	GipfAI solver;

public:
	Gipf(vector<vector<char>> board, int size, int pawnsCollect, int whiteMaxPawns, int blackMaxPawns,
		int whitePawns, int blackPawns, int turn);
	void operator=(Gipf& new_gipf);
	void operator=(Gipf&& new_gipf);
	Gipf(Gipf& other);
	Gipf(Gipf&& other);
	Gipf();

	std::pair<int, int> countChainsOnBoard();

	void print() override;
	void executeCommand(int command) override;
	int translateCommand(std::string command) override;
	vector<vector<char>> getBoard();

	char currentColor();
	GipfPlayer* currentPlayer();

	int getPawnsCollect();

	static vector<vector<char>> createBoard(int size);

	void loadGameBoard();
	bool loadBoardState();

	void putPawn(int x, int y);
	void endTurn();

	void doMove();
	void deletePossibleChains(int x, int y, std::pair<int, int>& pushVector, bool movedLine, std::string& start, std::string& end);
	
	void printBadMoveReason(int valid, std::string& pushSource, std::string& field);

	static void loadMoveSegments(std::string& move, std::string& start, std::string& dir);
	static std::pair<std::string, int> separateIndex(std::string& index);

	bool pushLine(std::string& pushSource, std::string& field, std::pair<int, int>& coords);
	bool pushLine(std::pair<int, int>& pushSource, std::pair<int, int>& field);

	std::pair<int, int> getCoordinates(std::string& index);
	std::pair<int, int> getPushVector(std::string& pushSource, std::string& field);
	std::pair<int, int> getPushVector(std::pair<int, int>& start, std::pair<int, int>& end);

	int moveValid(std::string& source, std::string& field);
	int checkIfWrongIndex(std::pair<int, int>& source_p, std::pair<int, int>& field_p);
	int checkIfUnknownDirection(std::pair<int, int>& source_p, std::pair<int, int>& field_p);
	int checkIfWrongStartingField(std::pair<int, int>& source_p);
	int checkIfWrongDestinationField(std::pair<int, int>& field_p);
	void setBadMoveState(std::string& pushSource, std::string& field);

	void printGameState();
	std::string getGameState();
	bool madeBadMove();
	bool isDeadLock();
	void setTurnCorrect();

	static bool insideBoard(vector<vector<char>>& checkedBoard, std::pair<int, int> coords);
	static bool insideBoard(vector<vector<char>>& checkedBoard, int col, int row);
	int countRowsInColumn(int index);
	int countRowStart(int index);
	
	std::pair<int, int> getChainsHorizontally();
	std::pair<int, int> getChainsVertically();
	std::pair<int, int> getChainsDiagonally();

	void handleChainsCountingAtLoading(int& col, int& row, int& whiteCount, int& blackCount,
		int& totalWhiteRows, int& totalBlackRows, int& mode);
	
	static void getMove(std::string& move, char& turn, std::string& start, std::string& end);
	
	// solver
	void printPossibleMoves();
};


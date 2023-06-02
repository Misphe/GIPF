#pragma once
#include "Game2D.h"
#include <iostream>
#include <vector>
#include <functional>
#include "Defines.h"
#include "GipfPlayer.h"
#include "GipfPointsManager.h"
#include "GipfAI.h"

using std::unique_ptr;
using std::vector;
using std::pair;

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
	Gipf& operator=(const Gipf& new_gipf);
	Gipf& operator=(Gipf&& new_gipf);
	bool operator<(const Gipf& other) const;
	bool operator==(const Gipf& other);
	bool operator==(const Gipf& other) const;
	Gipf(Gipf& other);
	Gipf(Gipf&& other);
	Gipf(const Gipf& other);
	Gipf();

	std::pair<int, int> countChainsOnBoard();
	bool errorGame();

	void print() const override;
	void executeCommand(int command) override;
	int translateCommand(std::string command) override;
	vector<vector<char>> getBoard() const;

	char currentColor() const;
	const GipfPlayer* currentPlayer() const;
	GipfPlayer* currentPlayer();

	int getPawnsCollect();
	GipfPointsManager* getManager();

	char getColor(pair<int, int> slot);

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
	std::pair<int, int> getPushVector(std::pair<int, int> start, std::pair<int, int> end);

	int moveValid(std::string& source, std::string& field);
	int checkIfWrongIndex(std::pair<int, int>& source_p, std::pair<int, int>& field_p);
	int checkIfUnknownDirection(std::pair<int, int>& source_p, std::pair<int, int>& field_p);
	int checkIfWrongStartingField(std::pair<int, int>& source_p);
	int checkIfWrongDestinationField(std::pair<int, int>& field_p);
	void setBadMoveState(std::string& pushSource, std::string& field);

	void printGameState();
	std::string getGameState() const;
	const bool madeBadMove() const;
	bool isDeadLock() const;
	void setTurnCorrect();

	static bool insideBoard(vector<vector<char>>& checkedBoard, std::pair<int, int> coords);
	static bool insideBoard(const vector<vector<char>>& checkedBoard, int col, int row);
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
	void printUniqueMovesNumber();
	void printUniqueMovesNumberExt();
	void printPossibleMovesExt();



	vector<vector<Chain>> getIntersectingChains(set<Chain>& chains);


	void clearIntersectionTable(vector<vector<bool>>& table, const Chain& baseChain);
	void fillIntersectionTable(vector<vector<bool>>& table, const Chain& chains);
	bool checkIfChainIntersect(vector<vector<bool>>& table, const Chain& checkedChain);


	~Gipf();
};

namespace std {
	template <>
	struct hash<Gipf> {
		size_t operator()(const Gipf& gipf) const {
			size_t seed = 0;
			for (const auto& row : gipf.getBoard()) {
				for (const auto& value : row) {
					seed ^= hash<char>{}(value)+0x9e3779b9 + (seed << 6) + (seed >> 2);
				}
			}
			return seed;
		}
	};
}
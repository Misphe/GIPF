#include "GipfAI.h"
#include "Gipf.h"
using std::pair;

GipfAI::GipfAI(Gipf& game) : game(&game) {}

void GipfAI::printAllPossibilities() {
	auto games = getAllPossibilities();

	for (int i = 0; i < games.size(); i++) {
		games[i].print();
	}
}

vector<Gipf> GipfAI::getAllPossibilities() {

	vector<pair<pair<int, int>, pair<int, int>>> allMoves = getAllPossibleMoveCommands();
	vector<Gipf> games;

	for (int i = 0; i < allMoves.size(); i++) {

		// moving generated game to the vector
		games.push_back(std::move(makeMove(allMoves[i].first, allMoves[i].second)));

		// if returned null game cuz of an bad move
		if (!games.back().isRunning()) {
			games.pop_back();
		}
	}

	return games;
}

// TODO 
// check for situations where two chains are possible to destroy
// but idk how yet
Gipf GipfAI::makeMove(pair<int, int>& pushSource, pair<int, int>& field) {
	static Gipf nullGame;
	Gipf fakeGame(*game);
	std::pair<int, int> pushVector = fakeGame.getPushVector(pushSource, field);
	auto board = fakeGame.getBoard();
	std::string start, end;


	int x = field.first;
	int y = field.second;
	auto boardCopy = board;
	bool movedLine = false;

	if (board[x][y] != EMPTYCELL) {
		bool success = fakeGame.pushLine(pushSource, field);
		movedLine = true;

		if (!success) {
			return Gipf();
		}
	}

	fakeGame.putPawn(x, y);
	fakeGame.deletePossibleChains(x, y, pushVector, movedLine, start, end);
	fakeGame.setTurnCorrect();
	fakeGame.endTurn();

	return fakeGame;
}

// dont question this
// generate all possible move indexes/commands
// currently with duplicates
vector<pair<pair<int, int>, pair<int, int>>> GipfAI::getAllPossibleMoveCommands() {
	auto board = game->getBoard();
	int size = board.size() / 2 + 1;
	int moves = 6 + (size - 1) * 2 * 6;
	vector <pair<pair<int, int>, pair<int, int>>> allMoves;

	// add left top corner
	allMoves.push_back({ { -1,-1 }, { 0,0 } });

	int boardCol, boardRow;
	int pushCol, pushRow;

	pushRow = -1;
	boardRow = 0;

	// add moves from left top to right top
	for (int col = 0; col < size - 1; col++) {
		allMoves.push_back({ {col, pushRow}, {col, boardRow} });
		allMoves.push_back({ {col, pushRow}, {col + 1, boardRow} });
	}

	allMoves.push_back({ { size - 1, pushRow }, { size - 1, boardRow } });

	// add moves from left top to left
	pushCol = -1;
	boardCol = 0;
	for (int row = 0; row < size - 1; row++) {
		allMoves.push_back({ {pushCol, row}, {boardCol, row} });
		allMoves.push_back({ {pushCol, row}, {boardCol, row + 1} });
	}

	allMoves.push_back({ { pushCol, size - 1}, { boardCol, size - 1 } });

	pushCol = 0;
	pushRow = size;
	boardCol = 0;
	boardRow = size - 1;

	// add moves from left to left bottom
	for (int i = 0; i < size - 1; i++) {
		allMoves.push_back({ {pushCol, pushRow}, {boardCol, boardRow} });
		allMoves.push_back({ {pushCol, pushRow}, {boardCol + 1, boardRow + 1} });
		pushCol++, pushRow++, boardCol++, boardRow++;
	}

	allMoves.push_back({ {pushCol, pushRow}, {boardCol, boardRow} });
	pushCol++, boardCol++;

	// add moves from left bottom to right bottom
	for (int i = 0; i < size - 1; i++) {
		allMoves.push_back({ {pushCol, pushRow}, {boardCol, boardRow} });
		allMoves.push_back({ {pushCol, pushRow}, {boardCol - 1, boardRow} });
		pushCol++, boardCol++;
	}

	boardCol--;
	allMoves.push_back({ {pushCol, pushRow}, {boardCol, boardRow} });
	pushRow--;

	// add moves from right bottom to right
	for (int i = 0; i < size - 1; i++) {
		allMoves.push_back({ {pushCol, pushRow}, {boardCol, boardRow} });
		allMoves.push_back({ {pushCol, pushRow}, {boardCol, boardRow - 1} });
		pushRow--, boardRow--;
	}

	allMoves.push_back({ {pushCol, pushRow}, {boardCol, boardRow} });
	pushCol--, pushRow--;

	// add moves from right to top right
	for (int i = 0; i < size - 1; i++) {
		allMoves.push_back({ {pushCol, pushRow}, {boardCol, boardRow} });
		allMoves.push_back({ {pushCol, pushRow}, {boardCol - 1, boardRow - 1} });
		pushCol--, pushRow--, boardCol--, boardRow--;
	}

	return allMoves;
}


#include "GipfAI.h"
#include "Gipf.h"
using std::pair;

GipfAI::GipfAI(Gipf& game) : game(&game) {}

void GipfAI::printAllPossibilities() {
	auto boards = getAllPossibilities();
}

vector<vector<vector<char>>> GipfAI::getAllPossibilities() {
	auto board = game->getBoard();
	int size = board.size() / 2 + 1;
	int moves = 6 + (size - 1) * 2 * 6;
	pair<int, int> start, end;

	// TODO
	// 1. generate all possible moves, e.g. a1-b2, a5-b5, etc.
	// 2. loop through all of them and call makeMove()
	// 3. be happy

	makeMove(start, end);

	return vector<vector<vector<char>>>();
}

vector<vector<char>> GipfAI::makeMove(pair<int, int>& pushSource, pair<int, int>& field) {
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
			return vector<vector<char>>();
		}
	}

	fakeGame.putPawn(x, y);
	fakeGame.deletePossibleChains(x, y, pushVector, movedLine, start, end);
	fakeGame.setTurnCorrect();
	fakeGame.endTurn();

	fakeGame.print();

	return board;
}


#include "GipfAI.h"
#include "Gipf.h"
#include <algorithm>
using std::pair;

struct Move {
	pair<int, int> start;
	pair<int, int> end;
	Move(pair<int, int> source, pair<int, int> field) : start(source), end(field) {}
};

GipfAI::GipfAI(Gipf& game) : game(&game) {}

void GipfAI::printAllPossibilities() {
	auto games = getAllPossibilities();

	for (auto& game : games) {
		game.print();
	}
}
 
 
void GipfAI::printUniqueMovesNumber() {
	auto games = getAllPossibilities();

	std::cout << games.size() << " UNIQUE_MOVES\n";
}

unordered_set<Gipf> GipfAI::getAllPossibilities() {

	vector<Move> allMoves = getAllPossibleMoveCommands();
	unordered_set<Gipf> games;

	for (int i = 0; i < allMoves.size(); i++) {

		// moving generated game to the vector
		Gipf game = std::move(makeMove(allMoves[i].start, allMoves[i].end));

		// check if returned null game
		if (!game.errorGame()) {
			games.insert(game);
		}

	}

	//deleteDuplicates(games);

	return games;
}

// TODO 
// check for situations where two chains are possible to destroy
// but idk how yet
Gipf GipfAI::makeMove(pair<int, int>& pushSource, pair<int, int>& field) {
	Gipf fakeGame(*game);
	pair<int, int> pushVector = fakeGame.getPushVector(pushSource, field);
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

	set<Chain> chains = std::move(fakeGame.getManager()->checkChains(x, y, pushVector, movedLine));
	char chainSymbol = NONE;

	switch (chains.size()) {
	case 0:
		break;
	default:
		for (const auto& chain : chains) {
			fakeGame.getManager()->deleteChain(chain.start, chain.end, board[chain.start.first][chain.start.second]);
		}
		break;
	}

	fakeGame.setTurnCorrect();
	fakeGame.endTurn();

	return fakeGame;
}

 
// dont question this
// generate all possible move indexes/commands
// currently with duplicates
// STOS IS THROWING ERROR BECAUSE OF EMPLACE_BACK/PUSH_BACK????????
vector<Move> GipfAI::getAllPossibleMoveCommands() {
	auto board = game->getBoard();
	int size = board.size() / 2 + 1;
	int moves = 6 + (size - 1) * 2 * 6;
	vector<Move> allMoves;

	// add left top corner
	// STOS error?????
	allMoves.emplace_back(std::pair<int, int>(-1, -1), std::pair<int, int>(0, 0));

	int boardCol, boardRow;
	int pushCol, pushRow;

	pushRow = -1;
	boardRow = 0;

	// add moves from left top to right top
	for (int col = 0; col < size - 1; col++) {
		allMoves.emplace_back(std::pair<int, int>(col, pushRow), std::pair<int, int>(col, boardRow));
		allMoves.emplace_back(std::pair<int, int>(col, pushRow), std::pair<int, int>(col + 1, boardRow));
	}

	allMoves.emplace_back(std::pair<int, int>(size - 1, pushRow), std::pair<int, int>(size - 1, boardRow));

	// add moves from left top to left
	pushCol = -1;
	boardCol = 0;
	for (int row = 0; row < size - 1; row++) {
		allMoves.emplace_back(std::pair<int, int>(pushCol, row), std::pair<int, int>(boardCol, row));
		allMoves.emplace_back(std::pair<int, int>(pushCol, row), std::pair<int, int>(boardCol, row + 1));
	}

	allMoves.emplace_back(std::pair<int, int>(pushCol, size - 1), std::pair<int, int>(boardCol, size - 1));

	pushCol = 0;
	pushRow = size;
	boardCol = 0;
	boardRow = size - 1;

	// add moves from left to left bottom
	for (int i = 0; i < size - 1; i++) {
		allMoves.emplace_back(std::pair<int, int>(pushCol, pushRow), std::pair<int, int>(boardCol, boardRow));
		allMoves.emplace_back(std::pair<int, int>(pushCol, pushRow), std::pair<int, int>(boardCol + 1, boardRow + 1));
		pushCol++, pushRow++, boardCol++, boardRow++;
	}

	allMoves.emplace_back(std::pair<int, int>(pushCol, pushRow), std::pair<int, int>(boardCol, boardRow));
	pushCol++, boardCol++;

	// add moves from left bottom to right bottom
	for (int i = 0; i < size - 1; i++) {
		allMoves.emplace_back(std::pair<int, int>(pushCol, pushRow), std::pair<int, int>(boardCol, boardRow));
		allMoves.emplace_back(std::pair<int, int>(pushCol, pushRow), std::pair<int, int>(boardCol - 1, boardRow));
		pushCol++, boardCol++;
	}

	boardCol--;
	allMoves.emplace_back(std::pair<int, int>(pushCol, pushRow), std::pair<int, int>(boardCol, boardRow));
	pushRow--;

	// add moves from right bottom to right
	for (int i = 0; i < size - 1; i++) {
		allMoves.emplace_back(std::pair<int, int>(pushCol, pushRow), std::pair<int, int>(boardCol, boardRow));
		allMoves.emplace_back(std::pair<int, int>(pushCol, pushRow), std::pair<int, int>(boardCol, boardRow - 1));
		pushRow--, boardRow--;
	}

	allMoves.emplace_back(std::pair<int, int>(pushCol, pushRow), std::pair<int, int>(boardCol, boardRow));
	pushCol--, pushRow--;

	// add moves from right to top right
	for (int i = 0; i < size - 1; i++) {
		allMoves.emplace_back(std::pair<int, int>(pushCol, pushRow), std::pair<int, int>(boardCol, boardRow));
		allMoves.emplace_back(std::pair<int, int>(pushCol, pushRow), std::pair<int, int>(boardCol - 1, boardRow - 1));
		pushCol--, pushRow--, boardCol--, boardRow--;
	}

	return allMoves;
}

void GipfAI::deleteDuplicates(vector<Gipf>& games) {
	games.erase(std::unique(games.begin(), games.end()), games.end());
}


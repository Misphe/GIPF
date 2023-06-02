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

void GipfAI::printUniqueMovesNumberExt() {
	auto games = getAllPossibilities();
	Gipf winning = std::move(getWinningMove(games));

	if (winning.isRunning()) {
		std::cout << "1_UNIQUE_MOVES\n";
	}

	else {
		std::cout << games.size() << "_UNIQUE_MOVES\n";
	}
}

void GipfAI::printPossibleMovesExt() {
	auto games = getAllPossibilities();
	Gipf winning = std::move(getWinningMove(games));

	if (winning.isRunning()) {
		winning.print();
	}

	else {
		for (auto& game_ : games) {
			game_.print();
		}
	}
}

void GipfAI::printAllPossibilities() {
	auto games = getAllPossibilities();

	for (auto& game_ : games) {
		game_.print();
	}
}
 
 
void GipfAI::printUniqueMovesNumber() {
	auto games = getAllPossibilities();

	std::cout << games.size() << "_UNIQUE_MOVES\n";
}

Gipf GipfAI::getWinningMove(unordered_set<Gipf>& games) {
	for (auto& game_ : games) {
		const GipfPlayer* player = game_.currentPlayer();
		if (player->getPawnsAmount() == 0 || game_.isDeadLock()) {
			return game_;
		}
	}
	return Gipf();
}

unordered_set<Gipf> GipfAI::getAllPossibilities() {

	vector<Move> allMoves = getAllPossibleMoveCommands();
	unordered_set<Gipf> games;

	for (int i = 0; i < allMoves.size(); i++) {

		// moving generated game to the vector
		vector<Gipf> game = std::move(makeMove(allMoves[i].start, allMoves[i].end));

		// check if error
		if (game.empty()) {
			continue;
		}

		// check if returned any null games
		for (int j = 0; j < game.size(); j++) {

			if (!game[j].errorGame()) {
				games.insert(game[j]);
			}
		}

	}

	return games;
}

// TODO
vector<Gipf> GipfAI::makeMove(pair<int, int>& pushSource, pair<int, int>& field) {
	Gipf fakeGame(*game);
	vector<Gipf> games;
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
			return vector<Gipf>();
		}
	}

	fakeGame.putPawn(x, y);

	set<Chain> chains = std::move(fakeGame.getManager()->checkChains(x, y, pushVector, movedLine));
	vector<vector<Chain>> intersectingChains = fakeGame.getIntersectingChains(chains);

	if (chains.size() > 0) {
		fakeGame.getManager()->deleteChains(chains, intersectingChains, x, y, pushVector, movedLine);
	}

	if (intersectingChains.size() == 0) {
		games.push_back(fakeGame);
	}

	for (auto& intersection : intersectingChains) {
		for (auto& chainInIntersection : intersection) {
			Gipf fakeGame2(fakeGame);
			fakeGame2.getManager()->deleteChain(chainInIntersection.start, chainInIntersection.end, chainInIntersection.color);

			/*chains = std::move(fakeGame2.getManager()->checkChains(x, y, pushVector, movedLine));
			intersectingChains = fakeGame2.getIntersectingChains(chains);*/

			games.push_back(fakeGame2);
		}
	}

	for (auto& tmp_game : games) {
		tmp_game.setTurnCorrect();
		tmp_game.endTurn();
	}

	return games;
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


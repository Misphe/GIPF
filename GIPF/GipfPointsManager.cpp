#include "GipfPointsManager.h"
#include "Gipf.h"
#include "Defines.h"
#include <set>
using std::vector;
using std::set;

GipfPointsManager::GipfPointsManager(Gipf& game) {
	this->game = &game;
}

set<Chain> GipfPointsManager::checkChains(int x, int y, std::pair<int, int>& pushVector, bool movedLine) {

	set<Chain> chains;
	int chainCount = 0;

	if (movedLine) {
		auto foundChains = std::move(checkMovedLine(x, y, pushVector));
		if (!foundChains.empty()) {
			chains = std::move(foundChains);
		}
	}
	else {
		Chain chain;
		chain = std::move(checkVertically(x, y));
		if (!chain.empty()) {
			chainCount++;
			chains.insert(std::move(chain));
		}
		chain = std::move(checkHorizontally(x, y));
		if (!chain.empty()) {
			chainCount++;
			chains.insert(std::move(chain));
		}
		chain = std::move(checkDiagonally(x, y));
		if (!chain.empty()) {
			chainCount++;
			chains.insert(std::move(chain));
		}
	}

	return chains;
}

void GipfPointsManager::checkChains(std::pair<int, int>& pushVector, std::string start_s, std::string end_s) {

	std::pair<int, int> direction = getPushVector(start_s, end_s);
	std::pair<int, int> start = game->getCoordinates(start_s);
	std::pair<int, int> end = game->getCoordinates(end_s);
	char color = game->board[start.first][start.second];

	deleteChain(start, end, color);

}

Chain GipfPointsManager::checkVertically(int col, int row) {

	vector<vector<char>>& board = game->board;
	int color = board[col][row];
	if (color == EMPTYCELL) {
		return Chain();
	}
	
	std::pair<int, int> start = findChainEnd(col, row, DOWN_DIR);
	std::pair<int, int> end = findChainEnd(col, row, UP_DIR);
	
	int length = abs(end.second - start.second) + 1;

	if (length < game->getPawnsCollect()) {
		return Chain();
	}

	return Chain(start, end, game->getColor(start));

	/*if (length >= game->getPawnsCollect()) {
		deleteChain(start, end, color);
	}*/
}

Chain GipfPointsManager::checkHorizontally(int col, int row) {

	vector<vector<char>>& board = game->board;
	int color = board[col][row];
	if (color == EMPTYCELL) {
		return Chain();
	}

	std::pair<int, int> start = findChainEnd(col, row, RIGHT_DIR);
	std::pair<int, int> end = findChainEnd(col, row, LEFT_DIR);

	int length = abs(end.first - start.first) + 1;

	if (length < game->getPawnsCollect()) {
		return Chain();
	}

	return Chain(start, end, game->getColor(start));

	/*if (length >= game->getPawnsCollect()) {
		deleteChain(start, end, color);
	}*/
}

Chain GipfPointsManager::checkDiagonally(int col, int row) {

	vector<vector<char>>& board = game->board;
	int color = board[col][row];
	if (color == EMPTYCELL) {
		return Chain();
	}

	std::pair<int, int> start = findChainEnd(col, row, DIAG_DOWN);
	std::pair<int, int> end = findChainEnd(col, row, DIAG_UP);

	int length = abs(end.second - start.second) + 1;

	if (length < game->getPawnsCollect()) {
		return Chain();
	}

	return Chain(start, end, game->getColor(start));

	/*if (length >= game->getPawnsCollect()) {
		deleteChain(start, end, color);
	}*/
}

set<Chain> GipfPointsManager::checkMovedLine(int col, int row, std::pair<int, int>& pushVector) {

	vector<vector<char>>& board = game->board;
	set<Chain> chains;
	Chain chainVer;
	Chain chainDiag;
	Chain chainHor;

	while (Gipf::insideBoard(board, col, row)) {
		chainVer = std::move(checkVertically(col, row));
		if (!chainVer.empty()) {
			chains.insert(std::move(chainVer));
		}
		chainHor = std::move(checkHorizontally(col, row));
		if (!chainHor.empty()) {
			chains.insert(std::move(chainHor));
		}
		chainDiag = std::move(checkDiagonally(col, row));
		if (!chainDiag.empty()) {
			chains.insert(std::move(chainDiag));
		}
		col += pushVector.first;
		row += pushVector.second;
	}

	return chains;
}

std::pair<int, int> GipfPointsManager::findChainEnd(int col, int row, int direction) {
	std::pair<int, int> pushVector(NONE, NONE);
	vector<vector<char>>& board = game->board;
	char color = board[col][row];
	if (color == EMPTYCELL) {
		return std::pair<int, int>(NULL, NULL);
	}
	
	switch (direction) {
	case DOWN_DIR:
		pushVector.second = DOWN;
		break;
	case UP_DIR:
		pushVector.second = UP;
		break;
	case RIGHT_DIR:
		pushVector.first = RIGHT;
		break;
	case LEFT_DIR:
		pushVector.first = LEFT;
		break;
	case DIAG_UP:
		pushVector.first = LEFT;
		pushVector.second = UP;
		break;
	case DIAG_DOWN:
		pushVector.first = RIGHT;
		pushVector.second = DOWN;
		break;
	}

	while (Gipf::insideBoard(board, col + pushVector.first, row + pushVector.second) && 
		board[col + pushVector.first][row + pushVector.second] == color) {
		col += pushVector.first;
		row += pushVector.second;
	}

	std::pair<int, int> end(col, row);
	return end;
}

void GipfPointsManager::deleteChain(const std::pair<int, int>& start, const std::pair<int, int>& end, char symbol) {
	int col = start.first;
	int row = start.second;
	vector<vector<char>>& board = game->board;
	std::pair<int, int> pushVectorFirst(NONE, NONE);
	std::pair<int, int> direction = getPushVector(start, end);

	pushVectorFirst.first = -direction.first;
	pushVectorFirst.second = -direction.second;

	deleteAdjacent(start, pushVectorFirst, symbol);
	deleteAdjacent(end, direction, symbol);
	deleteMiddle(start, end, direction, symbol);
}

void GipfPointsManager::deleteChains(set<Chain>& chains, vector<vector<Chain>>& intersectingChains, int x, int y,
	std::pair<int, int>& pushVector, bool movedLine) {
	while (!chains.empty()) {
		Chain chain = *chains.begin();
		deleteChain(chain.start, chain.end, chain.color);
		chains = std::move(checkChains(x, y, pushVector, movedLine));
		intersectingChains = game->getIntersectingChains(chains);
	}
}

void GipfPointsManager::deleteAdjacent(const std::pair<int, int>& start, std::pair<int, int>& dir, char symbol) {
	int col = start.first + dir.first;
	int row = start.second + dir.second;
	vector<vector<char>>& board = game->board;

	while (Gipf::insideBoard(board, col, row) && board[col][row] != EMPTYCELL) {
		switch (symbol) {
		case WHITEPAWN:

			if (board[col][row] == WHITEPAWN) {
				game->white.returnPawn();
			}

			break;
		case BLACKPAWN:

			if (board[col][row] == BLACKPAWN) {
				game->black.returnPawn();
			}

			break;
		}

		board[col][row] = EMPTYCELL;
		col += dir.first;
		row += dir.second;
	}
}

bool GipfPointsManager::chainCommandValid(char turn, std::string& start_s, std::string end_s) {
	turn = turn == 'w' ? WHITEPAWN : BLACKPAWN;
	std::pair<int, int> start = game->getCoordinates(start_s);
	std::pair<int, int> end = game->getCoordinates(end_s);
	std::pair<int, int> direction = getPushVector(start_s, end_s);
	std::pair<int, int> opp_direction = getPushVector(end_s, start_s);

	if (game->board[start.first][start.second] != turn || game->board[end.first][end.second] != turn) {
		std::cout << "WRONG_COLOR_OF_CHOSEN_ROW\n";
		return false;
	}

	int raw_direction = getDirection(direction);
	int opp_raw_direction = getDirection(opp_direction);
	std::pair<int, int> real_start = findChainEnd(start.first, start.second, opp_raw_direction);
	std::pair<int, int> real_end = findChainEnd(start.first, start.second, raw_direction);
	if (real_start != start || real_end != end) {
		std::cout << "WRONG_INDEX_OF_CHOSEN_ROW\n";
		return false;
	}

	return true;
}

void GipfPointsManager::deleteMiddle(const std::pair<int, int>& start, const std::pair<int, int>& end, 
	std::pair<int, int>& dir, char symbol) {
	int col = start.first;
	int row = start.second;
	vector<vector<char>>& board = game->board;
	GipfPlayer* player = symbol == WHITEPAWN ? &game->white : &game->black;

	while (col != end.first || row != end.second) {
		player->returnPawn();
		board[col][row] = EMPTYCELL;
		col += dir.first;
		row += dir.second;
	}

	player->returnPawn();
	board[col][row] = EMPTYCELL;
}

int GipfPointsManager::getDirection(std::pair<int, int>& direction) {
	if (direction.first == RIGHT && direction.second == NONE) {
		return RIGHT_DIR;
	}

	if (direction.first == LEFT && direction.second == NONE) {
		return LEFT_DIR;
	}

	if (direction.first == NONE && direction.second == UP) {
		return UP_DIR;
	}

	if (direction.first == NONE && direction.second == DOWN) {
		return DOWN_DIR;
	}

	if (direction.first == RIGHT && direction.second == DOWN) {
		return DIAG_DOWN;
	}

	if (direction.first == LEFT && direction.second == UP) {
		return DIAG_UP;
	}

	std::cout << "getDirection\n";
	return NONE;
}

int GipfPointsManager::directionToDimension(int direction) {
	if (direction == LEFT_DIR || direction == RIGHT_DIR) {
		return HORIZONTAL;
	}
	else if (direction == UP_DIR || direction == DOWN_DIR) {
		return VERTICAL;
	}
	else if (direction == DIAG_DOWN || direction == DIAG_UP) {
		return DIAGONAL;
	}

	std::cout << "error directionToDimension\n";
	return NONE;
}

std::pair<int, int> GipfPointsManager::getPushVector(std::string& start_s, std::string& end_s) {
	std::pair<int, int>start = game->getCoordinates(start_s);
	std::pair<int, int>end = game->getCoordinates(end_s);
	
	return getPushVector(start, end);
}

std::pair<int, int> GipfPointsManager::getPushVector(const std::pair<int, int>& start, const std::pair<int, int>& end) {
	std::pair<int, int> direction(NONE, NONE);

	if (start.first == end.first) {
		if (start.second > end.second) {
			direction = { NONE, UP };
		}
		else {
			direction = { NONE, DOWN };
		}
	}
	else if (start.second == end.second) {
		if (start.first > end.first) {
			direction = { LEFT, NONE };
		}
		else {
			direction = { RIGHT, NONE };
		}
	}
	else {
		if (start.first > end.first && start.second > end.second) {
			direction = { LEFT, UP };
		}
		else {
			direction = { RIGHT, DOWN };
		}
	}

	if (direction.first == NONE && direction.second == NONE) {
		std::cout << "error getPushVector gipfmanager\n";
	}

	return direction;
}




#include "GipfPointsManager.h"
#include "Gipf.h"
#include "Defines.h"
using std::vector;

struct Chain {
	std::pair<int, int> start;
	std::pair<int, int> end;
	Chain() {
		start = { EMPTY,EMPTY };
		end = { EMPTY,EMPTY };
	}
	Chain(std::pair<int, int>& start, std::pair<int, int>& end) {
		this->start = start;
		this->end = end;
	}
};

GipfPointsManager::GipfPointsManager(Gipf& game) {
	this->game = &game;
}

void GipfPointsManager::checkChains(int x, int y, std::pair<int, int>& pushVector, bool movedLine) {

	if (movedLine) {
		checkMovedLine(x, y, pushVector);
	}
	else {
		checkVertically(x, y);
		checkHorizontally(x, y);
		checkDiagonally(x, y);
	}

}

void GipfPointsManager::checkVertically(int col, int row) {

	vector<vector<char>>& board = game->board;
	int color = board[col][row];
	if (color == EMPTYCELL) {
		return;
	}
	
	std::pair<int, int> start = findChainEnd(col, row, DOWN_DIR);
	std::pair<int, int> end = findChainEnd(col, row, UP_DIR);
	
	int length = abs(end.second - start.second) + 1;
	if (length >= game->getPawnsCollect()) {
		deleteChain(start, end, color, VERTICAL);
	}
}

void GipfPointsManager::checkHorizontally(int col, int row) {

	vector<vector<char>>& board = game->board;
	int color = board[col][row];
	if (color == EMPTYCELL) {
		return;
	}

	std::pair<int, int> start = findChainEnd(col, row, RIGHT_DIR);
	std::pair<int, int> end = findChainEnd(col, row, LEFT_DIR);

	int length = abs(end.first - start.first) + 1;
	if (length >= game->getPawnsCollect()) {
		deleteChain(start, end, color, HORIZONTAL);
	}
}

void GipfPointsManager::checkDiagonally(int col, int row) {

	vector<vector<char>>& board = game->board;
	int color = board[col][row];
	if (color == EMPTYCELL) {
		return;
	}

	std::pair<int, int> start = findChainEnd(col, row, DIAG_DOWN);
	std::pair<int, int> end = findChainEnd(col, row, DIAG_UP);

	int length = abs(end.second - start.second) + 1;
	if (length >= game->getPawnsCollect()) {
		deleteChain(start, end, color, DIAGONAL);
	}
}

void GipfPointsManager::checkMovedLine(int col, int row, std::pair<int, int>& pushVector) {

	vector<vector<char>>& board = game->board;
	int direction = getDirection(pushVector);

	while (Gipf::insideBoard(board, col, row) && board[col][row] != EMPTYCELL) {
		checkVertically(col, row);
		checkHorizontally(col, row);
		checkDiagonally(col, row);
		col += pushVector.first;
		row += pushVector.second;
	}
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
		pushVector.first = DOWN;
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

void GipfPointsManager::deleteChain(std::pair<int, int>& start, std::pair<int, int>& end, char symbol, int direction) {
	int col = start.first;
	int row = start.second;
	vector<vector<char>>& board = game->board;
	std::pair<int, int> pushVectorFirst(NONE, NONE);
	std::pair<int, int> pushVectorSecond(NONE, NONE);
	std::pair<int, int> pushVectorMiddle(NONE, NONE);

	switch (direction) {
	case VERTICAL:
		pushVectorFirst.second = DOWN;
		pushVectorSecond.second = UP;
		pushVectorMiddle.second = UP;
		break;
	case HORIZONTAL:
		pushVectorFirst.first = RIGHT;
		pushVectorSecond.first = LEFT;
		pushVectorMiddle.first = LEFT;
		break;
	case DIAGONAL:
		pushVectorFirst.first = RIGHT;
		pushVectorFirst.second = DOWN;
		pushVectorSecond.first = LEFT;
		pushVectorSecond.second = UP;
		pushVectorMiddle.first = LEFT;
		pushVectorMiddle.second = UP;
		break;
	}

	deleteAdjacent(start, pushVectorFirst, symbol);
	deleteAdjacent(end, pushVectorSecond, symbol);
	deleteMiddle(start, end, pushVectorMiddle, symbol);
}

void GipfPointsManager::deleteAdjacent(std::pair<int, int>& start, std::pair<int, int>& dir, char symbol) {
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

void GipfPointsManager::deleteMiddle(std::pair<int, int>& start, std::pair<int, int>& end, 
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
		return RIGHT;
	}

	if (direction.first == LEFT && direction.second == NONE) {
		return LEFT;
	}

	if (direction.first == NONE && direction.second == UP) {
		return UP;
	}

	if (direction.first == NONE && direction.second == DOWN) {
		return DOWN;
	}

	if (direction.first == RIGHT && direction.second == DOWN) {
		return DIAG_DOWN;
	}

	if (direction.first == LEFT && direction.second == UP) {
		return DIAG_UP;
	}

	return NONE;
}




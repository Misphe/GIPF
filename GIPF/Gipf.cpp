#include "Gipf.h"
#include "Defines.h"

Gipf::Gipf(vector<vector<int>> board, int size) {
	this->board = board;
	this->size = size;
}

void Gipf::print() {
	for (int i = 0; i < board.size(); i++) {

		for (int k = 0; k < abs(size - 1 - i); k++) {
			std::cout << " ";
		}

		for (int j = 0; j < board[i].size(); j++) {
			if (board[i][j] != OUTOFMAP) {
				std::cout << board[i][j] << " ";
			}
		}
		std::cout << "\n";
	}
}

vector<vector<int>> Gipf::createBoard(int size) {
	vector<vector<int>> board;

	board.resize(size * 2 - 1);
	for (auto& col : board) {
		col.resize(size * 2 - 1, OUTOFMAP);
	}

	int col = 0;
	int startRow = 0;
	int fields = size;


	for (col = 0; col < size * 2 - 1; col++) {
		if (col >= size) {
			startRow++;
		}

		for (int i = startRow; i < startRow + fields; i++) {
			board[col][i] = EMPTYCELL;
		}

		if (col < size - 1) {
			fields++;
		}
		else {
			fields--;
		}

	}

	return board;
}

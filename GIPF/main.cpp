#include <iostream>
#include <memory>
#include <vector>
#include "Game.h"
#include "Gipf.h"

using std::unique_ptr;
using std::vector;

int main() {
	int size, pawnsCollect;
	int whitePawns, blackPawns;
	std::cin >> size >> pawnsCollect;
	std::cin >> whitePawns >> blackPawns;


	vector<vector<int>> board = Gipf::createBoard(size);
	unique_ptr<Game> game(new Gipf(board, size));
	game->print();
}
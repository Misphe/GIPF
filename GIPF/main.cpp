#include <iostream>
#include <memory>
#include <vector>
#include "Game2D.h"
#include "Gipf.h"
#include "Controller.h"

using std::unique_ptr;
using std::vector;

int main() {
	int size = 4, pawnsCollect = 4;
	int whiteMaxPawns = 15, blackMaxPawns = 15;
	int whitePawns = 4, blackPawns = 4;


	vector<vector<char>> board = Gipf::createBoard(size);
	Controller controller(new Gipf(board, size, pawnsCollect, whiteMaxPawns, blackMaxPawns, whitePawns, blackPawns, WHITETURN));

	while (controller.gameIsRunning()) {
		controller.passCommand();
	}
}
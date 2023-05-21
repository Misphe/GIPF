#pragma once
#include "Game.h"
#include <iostream>
#include <memory>
#include <vector>

using std::unique_ptr;
using std::vector;

class Gipf : public Game {
private:
	vector<vector<int>> board;
	int size;

public:
	Gipf(vector<vector<int>> board, int size);
	void print() override;


	static vector<vector<int>> createBoard(int size);
};


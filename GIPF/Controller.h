#pragma once
#include "Game2D.h"
#include <memory>

class Controller {
private:
	std::unique_ptr<Game2D> game;

public:
	Controller(Game2D* new_game);

	std::string getCommand();
	void passCommand();
	bool gameIsRunning();
};


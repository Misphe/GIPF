#pragma once
#include "Game.h"
#include <memory>

class Controller {
private:
	std::unique_ptr<Game> game;

public:
	Controller(Game* new_game);

	std::string getCommand();
	void passCommand();
	bool gameIsRunning();
};


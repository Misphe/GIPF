#pragma once
#include <string>

class Game {
protected:
	bool running;
	bool finished;

public:
	virtual void print() = 0;
	bool isRunning() {
		return running;
	}

	virtual int translateCommand(std::string command) = 0;
	virtual void executeCommand(int command) = 0;
};

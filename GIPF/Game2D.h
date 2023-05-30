#pragma once
#include <string>
#include <vector>

class Game2D {
protected:
	bool running;
	bool finished;

public:
	virtual void print() = 0;
	bool isRunning() const {
		return running;
	}

	virtual int translateCommand(std::string command) = 0;
	virtual void executeCommand(int command) = 0;
};

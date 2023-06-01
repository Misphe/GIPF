#pragma once
#include <string>

class Game2D {
protected:
	bool running;
	bool finished;

public:
	virtual void print() const = 0;
	bool isRunning() const {
		return running;
	}

	virtual int translateCommand(std::string command) = 0;
	virtual void executeCommand(int command) = 0;
	void end() {
		running = false;
	}

	virtual ~Game2D() {}
};

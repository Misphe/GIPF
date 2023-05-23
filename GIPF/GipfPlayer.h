#pragma once

class GipfPlayer {
private:
	int pawnsAmount;
	int maxPawns;
	bool alive;
	char symbol;

public:
	GipfPlayer(int startingpawnsAmount, int maxPawns, char symbol) : symbol(symbol) {
		this->pawnsAmount = startingpawnsAmount;
		this->maxPawns = maxPawns;
		this->alive = true;
	}

	void set(int set_pawnsAmount, int set_maxPawns) {
		this->pawnsAmount = set_pawnsAmount;
		this->maxPawns = set_maxPawns;
	}

	void operator=(GipfPlayer& other) {
		this->pawnsAmount = other.pawnsAmount;
		this->maxPawns = other.maxPawns;
		this->symbol = other.symbol;
	}

	int getUsedPawns() {
		return maxPawns - pawnsAmount;
	}

	bool lost() {
		return alive;
	}

	int getMaxPawns() {
		return maxPawns;
	}

	char getPawnsSymbol() {
		return symbol;
	}

	int getPawnsAmount() {
		return pawnsAmount;
	}

	void usePawn() {
		if (pawnsAmount == 0) {
			return;
		}

		pawnsAmount--;

		if (pawnsAmount == 0) {
			alive = false;
		}
	}
};
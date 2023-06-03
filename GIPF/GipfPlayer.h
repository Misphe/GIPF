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
		if (startingpawnsAmount > 0) {
			this->alive = true;
		}
		else {
			this->alive = false;
		}
	}

	void set(int set_pawnsAmount, int set_maxPawns) {
		this->pawnsAmount = set_pawnsAmount;
		this->maxPawns = set_maxPawns;
	}

	GipfPlayer& operator=(const GipfPlayer& other) {
		this->pawnsAmount = other.pawnsAmount;
		this->maxPawns = other.maxPawns;
		this->symbol = other.symbol;
		this->alive = other.alive;
		return *this;
	}

	int getUsedPawns() const {
		return maxPawns - pawnsAmount;
	}

	bool lost() const {
		if (!alive) {
			return true;
		}
		return false;
	}

	int getMaxPawns() const {
		return maxPawns;
	}

	char getPawnsSymbol() const {
		return symbol;
	}

	int getPawnsAmount() const {
		return pawnsAmount;
	}

	void returnPawn() {
		pawnsAmount++;
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
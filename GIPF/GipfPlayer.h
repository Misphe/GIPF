#pragma once

class GipfPlayer {
private:
	int pawnAmount;
	int maxPawns;
	char symbol;

public:
	GipfPlayer(int startingPawnAmount, int maxPawns, char symbol) : symbol(symbol) {
		this->pawnAmount = startingPawnAmount;
		this->maxPawns = maxPawns;
	}

	void set(int set_pawnAmount, int set_maxPawns) {
		this->pawnAmount = set_pawnAmount;
		this->maxPawns = set_maxPawns;
	}

	void operator=(GipfPlayer& other) {
		this->pawnAmount = other.pawnAmount;
		this->maxPawns = other.maxPawns;
		this->symbol = other.symbol;
	}

	int getUsedPawns() {
		return maxPawns - pawnAmount;
	}

	char getPawnsSymbol() {
		return symbol;
	}

	void usePawn() {
		pawnAmount--;
	}
};
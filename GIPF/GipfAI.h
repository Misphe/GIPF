#pragma once
#include <iostream>
#include <vector>
#include <unordered_set>
#include "Chain.h"
using std::vector;
using std::pair;
using std::unordered_set;

class Gipf;
struct Move;

class GipfAI {
private:
	Gipf* mainGame;

public:
	GipfAI(Gipf& game);

	void printAllPossibilities();
	void printUniqueMovesNumber();
	void printUniqueMovesNumberExt();
	void printPossibleMovesExt();
	void printSolvedGameState(int moves);



	std::string getPossibleStateInNTurns(int moves);
	void getAllPossibilitiesInNTurns(int moves, int moveCount, Gipf& game, vector<Gipf>& possibilities);
	Gipf getWinningMove(unordered_set<Gipf>& games);
	unordered_set<Gipf> getAllPossibilities(const Gipf& checkedGame);
	vector<Gipf> makeMove(pair<int, int>& pushSource, pair<int, int>& field, const Gipf& checkedGame);

	// here is stos error
	vector<Move> getAllPossibleMoveCommands();
	
	static void deleteDuplicates(vector<Gipf>& games);

	vector<vector<Chain>> getChainCombinations(vector<vector<Chain>>& intersectingChains);
	void handleIntersectionsCombinations(vector<Gipf>& games, Gipf& fakeGame, vector<vector<Chain>>& intersectingChains,
		int x, int y, std::pair<int, int>& pushVector, bool movedLine);
	void generateChainCombinations(vector<vector<Chain>>&  intersectingChains, vector<vector<Chain>>& combinations, 
		vector<Chain>& combination, int currentPos);
};


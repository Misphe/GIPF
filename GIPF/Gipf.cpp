#include "Gipf.h"
#include "Defines.h"
#include "GipfPointsManager.h"
#include <string>
#include <iostream>

using std::pair;

template<typename T>
bool vectorsEqual(const std::vector<T>& v1, const std::vector<T>& v2) {
	if (v1.size() != v2.size()) {
		return false;
	}

	for (int i = 0; i < v1.size(); ++i) {
		if (v1[i] != v2[i]) {
			return false;
		}
	}

	return true;
}

using std::cin;

Gipf::Gipf(vector<vector<char>> board, int size, int pawnsCollect, int whiteMaxPawns, int blackMaxPawns,
	int whitePawns, int blackPawns, int turn) 
	: black(blackPawns, blackMaxPawns, BLACKPAWN), white(whitePawns, whiteMaxPawns, WHITEPAWN),
	manager(*this), solver(*this) {
	this->board = board;
	this->size = size;
	this->running = true;
	this->boardEmpty = false;
	this->badMove.first = false;
	this->turn = turn;
	this->pawnsCollect = pawnsCollect;
	this->finished = false;
	this->state = IN_PROGRESS;
}

Gipf::Gipf(Gipf& other) : manager(*this), solver(*this),
black(other.black.getPawnsAmount(), other.black.getMaxPawns(), other.black.getPawnsSymbol()),
white(other.white.getPawnsAmount(), other.white.getMaxPawns(), other.white.getPawnsSymbol()) {
	this->board = other.board;
	this->size = other.size;
	this->running = other.running;
	this->boardEmpty = other.boardEmpty;
	this->badMove.first = other.badMove.first;
	this->turn = other.turn;
	this->pawnsCollect = other.pawnsCollect;
	this->finished = other.isRunning();
	this->state = other.getGameState();
}

Gipf::Gipf(const Gipf& other) : manager(*this), solver(*this),
black(other.black.getPawnsAmount(), other.black.getMaxPawns(), other.black.getPawnsSymbol()),
white(other.white.getPawnsAmount(), other.white.getMaxPawns(), other.white.getPawnsSymbol()) {
	this->board = other.board;
	this->size = other.size;
	this->running = other.running;
	this->boardEmpty = other.boardEmpty;
	this->badMove.first = other.badMove.first;
	this->turn = other.turn;
	this->pawnsCollect = other.pawnsCollect;
	this->finished = other.isRunning();
	this->state = IN_PROGRESS;
}

Gipf::Gipf(Gipf&& other) : manager(*this), solver(*this),
black(other.black.getPawnsAmount(), other.black.getMaxPawns(), other.black.getPawnsSymbol()),
white(other.white.getPawnsAmount(), other.white.getMaxPawns(), other.white.getPawnsSymbol()) {
	std::swap(this->board, other.board);
	this->size = other.size;
	this->running = other.running;
	this->boardEmpty = other.boardEmpty;
	this->badMove.first = other.badMove.first;
	this->turn = other.turn;
	this->pawnsCollect = other.pawnsCollect;
	this->finished = other.isRunning();
	this->state = other.getGameState();
}

Gipf::Gipf() : black(NULL, NULL, NULL), white(NULL, NULL, NULL), manager(*this), solver(*this) {
	running = false;
}

Gipf& Gipf::operator=(const Gipf& new_gipf) {
	this->board = new_gipf.board;
	this->size = new_gipf.size;
	this->running = new_gipf.running;
	this->turn = new_gipf.turn;
	this->pawnsCollect = new_gipf.pawnsCollect;
	this->white = new_gipf.white;
	this->black = new_gipf.black;
	return *this;
}

Gipf& Gipf::operator=(Gipf&& other) {
	std::swap(this->board, other.board);
	this->size = other.size;
	this->running = other.running;
	this->turn = other.turn;
	this->pawnsCollect = other.pawnsCollect;
	this->white = other.white;
	this->black = other.black;
	return *this;
}

bool Gipf::operator<(const Gipf& other) const {
	for (int i = 0; i < board.size(); i++) {
		if (board[i] != other.board[i]) {
			return false;
		}
	}
	return true;
}

bool Gipf::operator==(const Gipf& other) {
	if (vectorsEqual(board, other.board) && white.getPawnsAmount() == other.white.getPawnsAmount() &&
		black.getPawnsAmount() == other.black.getPawnsAmount()) {
		return true;
	}
	return false;
}

bool Gipf::operator==(const Gipf& other) const {
	return board == other.board;
}

std::pair<int, int> Gipf::countChainsOnBoard() {

	std::pair<int, int> sum;
	std::pair<int, int> tmp;
	tmp = getChainsHorizontally();
	sum.first += tmp.first;
	sum.second += tmp.second;
	tmp = getChainsVertically();
	sum.first += tmp.first;
	sum.second += tmp.second;
	tmp = getChainsDiagonally();
	sum.first += tmp.first;
	sum.second += tmp.second;

	return std::pair<int, int>(sum.first, sum.second);
}

bool Gipf::errorGame() {
	return board.size() == 0;
}

void Gipf::print() const {

	if (boardEmpty) {
		std::cout << "EMPTY_BOARD\n";
		return;
	}

	std::cout << size << " " << pawnsCollect << " " << white.getMaxPawns() <<
		" " << black.getMaxPawns() << "\n";

	std::cout << white.getPawnsAmount() << " " << black.getPawnsAmount() << " " << currentColor() << "\n";

	for (int i = 0; i < board.size(); i++) {

		for (int k = 0; k < abs(size - 1 - i); k++) {
			std::cout << " ";
		}

		for (int j = 0; j < board.size(); j++) {
			if (board[j][i] != OUTOFMAP) {
				std::cout << board[j][i] << " ";
			}
		}

		std::cout << "\n";
	}

	int x = 0;
}

// TODO
void Gipf::executeCommand(int command) {
	switch (command) {
		case LOAD_GAME_BOARD:
			loadGameBoard();
			break;
		case PRINT_GAME_BOARD:
			print();
			break;
		case DO_MOVE:
			doMove();
			break;
		case PRINT_GAME_STATE:
			printGameState();
			break;
		case IS_GAME_OVER:
			isGameOver();
			break;
		case GEN_ALL_POS_MOV:
			printPossibleMoves();
			break;
		case GEN_ALL_POS_MOV_EXT:
			printPossibleMovesExt();
			break;
		case GEN_ALL_POS_MOV_NUM:
			printUniqueMovesNumber();
			break;
		case GEN_ALL_POS_MOV_EXT_NUM:
			printUniqueMovesNumberExt();
			break;
		case WINNING_SEQUENCE_EXIST:

			break;
		case SOLVE_GAME_STATE:
			printSolvedGameState();
			break;
		default:

			break;
	}
}

void Gipf::printPossibleMoves() {
	solver.printAllPossibilities();
}

void Gipf::printUniqueMovesNumber() {
	solver.printUniqueMovesNumber();
}

void Gipf::printUniqueMovesNumberExt() {
	solver.printUniqueMovesNumberExt();
}

void Gipf::printPossibleMovesExt() {
	solver.printPossibleMovesExt();
}

void Gipf::printSolvedGameState() {
	int moves;
	cin >> moves;
	if (moves > 4) moves = 4;
	if (moves < 1) moves = 1;
	solver.printSolvedGameState(moves);
}

vector<vector<Chain>> Gipf::getIntersectingChains(set<Chain>& chains) {
	if (chains.size() < 2) {
		return vector<vector<Chain>>();
	}

	// mark which chains are intersecting and remove them from chains set
	set<Chain> markedChains;

	vector<vector<Chain>> intersectingChains;
	vector<vector<bool>> table(board.size(), std::vector<bool>(board.size(), false));

	// control vector indexes
	int index = 0;
	int intersectionInIndex = 0;

	for (auto it = chains.begin(); it != chains.end(); ++it) {
		auto& baseChain = *it;

		fillIntersectionTable(table, baseChain);


		for (auto it2 = it; it2 != chains.end(); ++it2) {
			auto& checkedChain = *it2;

			if (checkedChain == baseChain) {
				continue;
			}

			bool intersect = checkIfChainIntersect(table, checkedChain);
			if (intersect) {
				if (markedChains.count(checkedChain) > 0) {
					continue;
				}

				if (intersectionInIndex == 0) {
					intersectingChains.push_back(vector<Chain>());
					intersectingChains[index].push_back(baseChain);
					markedChains.insert(baseChain);
				}
				intersectingChains[index].push_back(checkedChain);
				markedChains.insert(checkedChain);
				intersectionInIndex++;
			}

		}

		if (intersectionInIndex > 0) {
			index++;
		}
		intersectionInIndex = 0;

		clearIntersectionTable(table, baseChain);
	}

	for (const auto& chain : markedChains) {
		chains.erase(chain);
	}

	return intersectingChains;
}

void Gipf::clearIntersectionTable(vector<vector<bool>>& table, const Chain& baseChain) {
	pair<int, int> push = getPushVector(baseChain.start, baseChain.end);
	pair<int, int> current = baseChain.start;

	while (current != baseChain.end) {
		table[current.first][current.second] = false;
		current.first += push.first;
		current.second += push.second;
	}
	table[current.first][current.second] = false;
}

void Gipf::fillIntersectionTable(vector<vector<bool>>& table, const Chain& baseChain) {
	pair<int, int> push = getPushVector(baseChain.start, baseChain.end);
	pair<int, int> current = baseChain.start;

	while (current != baseChain.end) {
		table[current.first][current.second] = true;
		current.first += push.first;
		current.second += push.second;
	}
	table[current.first][current.second] = true;
}

bool Gipf::checkIfChainIntersect(vector<vector<bool>>& table, const Chain& checkedChain) {
	pair<int, int> current = checkedChain.start;
	pair<int, int> push = getPushVector(checkedChain.start, checkedChain.end);

	while (current != checkedChain.end) {
		if (table[current.first][current.second] == true) {
			return true;
		}

		current.first += push.first;
		current.second += push.second;
	}
	if (table[current.first][current.second] == true) {
		return true;
	}

	return false;
}


void Gipf::printGameState() {
	std::cout << getGameState() << "\n";
}

void Gipf::isGameOver() const {
	std::string currentState;
	if (isDeadLock()) {
		currentState = currentColor() == WHITEPAWN ? BLACKWON : WHITEWON;
	}
	else if (white.lost() && currentPlayer() == &white) {
		currentState = BLACKWON;
	}
	else if (black.lost() && currentPlayer() == &black) {
		currentState = WHITEWON;
	}
	else {
		currentState = IN_PROGRESS;
	}
	std::cout << currentState << "\n";
}

std::string Gipf::getGameState() const {
	std::string currentState;
	if (madeBadMove()) {
		currentState = "BAD_MOVE " + std::string(1, currentColor()) + " " + badMove.second;
	}
	else if (white.lost() && currentPlayer() == &white) {
		currentState = BLACKWON;
	}
	else if (black.lost() && currentPlayer() == &black) {
		currentState = WHITEWON;
	}
	else if (isDeadLock()) {
		currentState = "DEAD_LOCK " + std::string(1, currentColor());
	}
	else {
		currentState = IN_PROGRESS;
	}
	return currentState;
}

const bool Gipf::madeBadMove() const {
	return badMove.first;
}

bool Gipf::isDeadLock() const {
	for (int row = 0; row < board.size(); row++) {
		for(int col = 0; col < board.size(); col++) {
			if (!insideBoard(board, col, row)) {
				continue;
			}

			if (board[col][row] == EMPTYCELL) {
				return false;
			}
		}
	}

	return true;
}

void Gipf::setTurnCorrect() {
	badMove.first = false;
}

vector<vector<char>> Gipf::createBoard(int size) {
	vector<vector<char>> board;
	size = size;

	board.resize(size * 2 - 1);
	for (auto& col : board) {
		col.resize(size * 2 - 1, OUTOFMAP);
	}

	int col = 0;
	int startRow = 0;
	int fields = size;


	for (col = 0; col < size * 2 - 1; col++) {
		if (col >= size) {
			startRow++;
		}

		for (int i = startRow; i < startRow + fields; i++) {
			board[col][i] = EMPTYCELL;
		}

		if (col < size - 1) {
			fields++;
		}
		else {
			fields--;
		}

	}

	return board;
}

bool Gipf::loadBoardState() {
	board = createBoard(size);

	bool WRONG_BOARD_ROW_LENGTH = false, success = false;
	int whiteCount = 0, blackCount = 0;
	bool exitProgramm = false;
	int row = 0, col = 0, colStart = 0;
	int rowSlots = size - 1, usedSlots;

	if (white.getUsedPawns() == white.getMaxPawns() && black.getUsedPawns() == black.getMaxPawns()) {
		return true;
	}
	getchar();
	for (int row = 0; row < board.size(); row++) {

		colStart += (row >= size ? 1 : 0);
		rowSlots += (row < size ? 1 : -1);

		usedSlots = 0;
		col = colStart;

		while (true) {

			char field = getchar();
			if (field == EOF) {
				exitProgramm = true;
				break;
			}

			if (field == ' ') {
				continue;
			}

			if (field == '\n' && usedSlots != rowSlots) {
				WRONG_BOARD_ROW_LENGTH = true;
				break;
			}
			else if (field == '\n') {
				break;
			}

			if (usedSlots < rowSlots) {
				board[col][row] = field;
				usedSlots++;
			}
			else {
				WRONG_BOARD_ROW_LENGTH = true;
			}

			if (field == BLACKPAWN) {
				blackCount++;
			}
			else if (field == WHITEPAWN) {
				whiteCount++;
			}

			col++;
		}
	}

	std::pair<int, int> rows = countChainsOnBoard();
	int sum = rows.first + rows.second;

	if (sum != 0) {
		if (sum == 1) {
			std::cout << "ERROR_FOUND_" + std::to_string(sum) + "_ROW_OF_LENGTH_K\n";
		}
		// plural form??
		else if (sum > 1) {
			std::cout << "ERROR_FOUND_" + std::to_string(sum) + "_ROWS_OF_LENGTH_K\n";
		}
	}
	else if (whiteCount > white.getUsedPawns()) {
		std::cout << "WRONG_WHITE_PAWNS_NUMBER\n";
	}
	else if (blackCount > black.getUsedPawns()) {
		std::cout << "WRONG_BLACK_PAWNS_NUMBER\n";
	}
	else if (WRONG_BOARD_ROW_LENGTH) {
		std::cout << "WRONG_BOARD_ROW_LENGTH\n";
	}
	else {
		success = true;
		std::cout << "BOARD_STATE_OK\n";
	}


	if (exitProgramm) {
		exit(0);
	}

	return success;
}

void Gipf::putPawn(int x, int y) {

	GipfPlayer* player = turn == WHITETURN ? &white : &black;
	board[x][y] = player->getPawnsSymbol();
	player->usePawn();

}

void Gipf::endTurn() {
	turn = turn == WHITETURN ? BLACKTURN : WHITETURN;
}

int Gipf::checkIfWrongIndex(std::pair<int, int>& source_p, std::pair<int, int>& field_p) {
	auto tmp = createBoard(size + 1);

	// a helping board to ilustrate how it works
	// -1 00 01 02 03 04 05
	// a4 b5 c6 d7 -1 -1 -1  |-1
	// a3 b4 c5 d6 e6 -1 -1  |00
	// a2 b3 c4 d5 e5 f5 -1  |01
	// a1 b2 c3 d4 e4 f4 g4  |02
	// -1 b1 c2	d3 e3 f3 g3  |03
	// -1 -1 c1 d2 e2 f2 g2  |04
	// -1 -1 -1 d1 e1 f1 g1  |05

	std::pair<int, int> tmp_p = source_p;
	tmp_p.first++;
	tmp_p.second++;

	// first coords
	if (!insideBoard(tmp, tmp_p)) {
		return BAD_MOVE_FIRST_IS_WRONG_INDEX;
	}

	if (!insideBoard(tmp, field_p)) {
		return BAD_MOVE_SECOND_IS_WRONG_INDEX;
	}

	return NOERRORS;
}

int Gipf::checkIfUnknownDirection(std::pair<int,int>& source_p, std::pair<int, int>& field_p) {

	// coords are not next to each other
	if (abs(source_p.first - field_p.first) > 1) {
		return UNKNOWN_MOVE_DIRECTION;
	}
	if (abs(source_p.second - field_p.second) > 1) {
		return UNKNOWN_MOVE_DIRECTION;
	}

	return NOERRORS;
}

int Gipf::checkIfWrongStartingField(std::pair<int, int>& source_p) {
	
	// if start is inside the normal board, it is wrongly placed then
	if (insideBoard(board, source_p)) {
		return BAD_MOVE__IS_WRONG_STARTING_FIELD;
	}

	return NOERRORS;
}

int Gipf::checkIfWrongDestinationField(std::pair<int, int>& field_p) {

	// if start is inside the normal board, it is wrongly placed then
	if (!insideBoard(board, field_p)) {
		return BAD_MOVE__IS_WRONG_DESTINATION_FIELD;
	}

	return NOERRORS;
}

int Gipf::moveValid(std::string& source, std::string& field) {

	std::pair<int, int> source_p = getCoordinates(source);
	std::pair<int, int> field_p = getCoordinates(field);
	int wrongIndex = checkIfWrongIndex(source_p, field_p);
	if (wrongIndex != NOERRORS) {
		return wrongIndex;
	}

	int unknownDirection = checkIfUnknownDirection(source_p, field_p);
	if (unknownDirection != NOERRORS) {
		return unknownDirection;
	}

	int wrongStartingField = checkIfWrongStartingField(source_p);
	if (wrongStartingField != NOERRORS) {
		return wrongStartingField;
	}

	int wrongDestinationField = checkIfWrongDestinationField(field_p);
	if (wrongDestinationField != NOERRORS) {
		return wrongDestinationField;
	}

	return NOERRORS;
}

void Gipf::setBadMoveState(std::string& pushSource, std::string& field) {
	state = BAD_MOVE;
	state = state + ' ' + currentColor() + ' ' + pushSource + "-" + field;

	badMove.first = true;
	badMove.second = pushSource + ' ' + field;
}

void Gipf::doMove() {

	std::string pushSource, field;
	std::string move, start, end;
	char chainTurn;
	cin >> move;

	loadMoveSegments(move, pushSource, field);

	std::pair<int, int> coords = getCoordinates(field);
	int x = coords.first;
	int y = coords.second;

	int valid = moveValid(pushSource, field);
	if (valid != NOERRORS) {
		setBadMoveState(pushSource, field);
		printBadMoveReason(valid, pushSource, field);
		return;
	}

	std::pair<int, int> pushVector = getPushVector(pushSource, field);
	auto boardCopy = board;
	bool movedLine = false;

	if (board[x][y] != EMPTYCELL) {
		bool success = pushLine(pushSource, field, coords);
		movedLine = true;

		if (!success) {
			std::cout << "BAD_MOVE_ROW_IS_FULL\n";
			return;
		}
	}

	putPawn(x, y);

	set<Chain> chains = std::move(manager.checkChains(x, y, pushVector, movedLine));
	vector<vector<Chain>> intersectingChains = getIntersectingChains(chains);

	char chainSymbol = NONE;

	if (chains.size() > 0) {
		manager.deleteChains(chains, intersectingChains, x, y, pushVector, movedLine, currentColor());
	}

	// delete intersecting chains based on users input
	while (stillSomethingToDestroy(intersectingChains, currentColor())) {
		if (deleteWrittenIntersections(intersectingChains, chains, x, y, pushVector, movedLine, boardCopy) == false) {
			return;
		}
	}

	if (chains.size() > 0) {
		manager.deleteChains(chains, intersectingChains, x, y, pushVector, movedLine);
	}

	while (stillSomethingToDestroy(intersectingChains, nextColor())) {
		if (deleteWrittenIntersections(intersectingChains, chains, x, y, pushVector, movedLine, boardCopy) == false) {
			return;
		}
	}

	std::cout << "MOVE_COMMITTED\n";
	badMove.first = false;
	endTurn();
}

bool Gipf::stillSomethingToDestroy(vector<vector<Chain>>& intersectingChains, char symbol) {
	for (auto& intersection : intersectingChains) {
		for (auto& checkedChain : intersection) {
			if (checkedChain.color == symbol) {
				return true;
			}
		}
	}
	return false;
}

// returns false if error
bool Gipf::deleteWrittenIntersections(vector<vector<Chain>>& intersectingChains, set<Chain>& chains,
	int x, int y, std::pair<int, int>& pushVector, bool movedLine, vector<vector<char>>& boardCopy) {
	std::string move, start, end;
	char chainTurn;

	cin >> chainTurn >> start >> start >> end;
	chainTurn = chainTurn == 'w' ? WHITEPAWN : BLACKPAWN;
	std::pair<int, int> startOfChain = getCoordinates(start);
	std::pair<int, int> endOfChain = getCoordinates(end);
	bool found = false;
	for (auto& intersection : intersectingChains) {
		for (auto& checkedChain : intersection) {
			if ((checkedChain.start == startOfChain && checkedChain.end == endOfChain) ||
				checkedChain.start == endOfChain && checkedChain.end == startOfChain) {
				found = true;

				if (chainTurn != getColor(startOfChain)) {
					std::cout << "WRONG_COLOR_OF_CHOSEN_ROW\n";
					std::swap(board, boardCopy);
					currentPlayer()->returnPawn();
					badMove.first = true;
					return false;
				}
				else {
					manager.deleteChain(startOfChain, endOfChain, chainTurn);
					chains = std::move(manager.checkChains(x, y, pushVector, movedLine));
					intersectingChains = getIntersectingChains(chains);
					break;
				}
			}
		}
	}

	if (!found) {
		std::cout << "WRONG_INDEX_OF_CHOSEN_ROW\n";
		std::swap(board, boardCopy);
		currentPlayer()->returnPawn();
		badMove.first = true;
		return false;
	}

	return true;
}

void Gipf::printBadMoveReason(int valid, std::string& pushSource, std::string& field) {
	switch (valid) {
	case BAD_MOVE_FIRST_IS_WRONG_INDEX:
		std::cout << "BAD_MOVE_" << pushSource << "_IS_WRONG_INDEX\n";
		return;
		break;
	case BAD_MOVE_SECOND_IS_WRONG_INDEX:
		std::cout << "BAD_MOVE_" << field << "_IS_WRONG_INDEX\n";
		return;
		break;
	case UNKNOWN_MOVE_DIRECTION:
		std::cout << "UNKNOWN_MOVE_DIRECTION\n";
		return;
		break;
	case BAD_MOVE__IS_WRONG_STARTING_FIELD:
		std::cout << "BAD_MOVE_" << pushSource << "_IS_WRONG_STARTING_FIELD\n";
		return;
		break;
	case BAD_MOVE__IS_WRONG_DESTINATION_FIELD:
		std::cout << "BAD_MOVE_" << field << "_IS_WRONG_DESTINATION_FIELD\n";
		return;
		break;
	}
}

void Gipf::deletePossibleChains(int x, int y, std::pair<int, int>& pushVector, bool movedLine,
	std::string& start, std::string& end) {
	if (end.empty()) {
		manager.checkChains(x, y, pushVector, movedLine);
	}
	else {
		manager.checkChains(pushVector, start, end);
	}
}

void Gipf::loadMoveSegments(std::string& move, std::string& first, std::string& second) {
	int i = 0;
	while (move.at(i) != '-') {
		first += move.at(i);
		i++;
	}

	i++;

	while (i < move.size()) {
		second += move.at(i);
		i++;
	}
}

std::pair<int, int> Gipf::getCoordinates(std::string& index) {
	std::pair<std::string, int> separated = separateIndex(index);
	std::string letter = separated.first;
	int number = separated.second;

	// transfer to ints
	int int_letter = 0;
	int i;
	for (i = 0; i < letter.size(); i++) {
		int_letter = int_letter + (letter.at(i) - 'a');
	}
	int_letter += ((i - 1) * ('z' - 'a'));

	// calculate coords
	int index1, index2, rows = size;
	index1 = int_letter - 1;

	rows = countRowsInColumn(index1);

	index2 = rows - number + 1 + countRowStart(index1);

	return std::pair<int, int>(index1, index2);
}

bool Gipf::pushLine(std::string& pushSource, std::string& field, std::pair<int, int>& coords) {

	// vector of push of the line attached to the coords
	int dx = 0;
	int dy = 0;
	int x = coords.first, y = coords.second;
	bool noSpace = true;

	if (board[x][y] == EMPTYCELL) {
		return true;
	}

	std::pair<int, int> translations = getPushVector(pushSource, field);
	dx = translations.first;
	dy = translations.second;

	while (x + dx < board.size() && y + dy < board.size() && x + dx >= 0 && y + dy >= 0 &&
		board[x + dx][y + dy] != OUTOFMAP) {

		if (board[x][y] == EMPTYCELL) {
			noSpace = false;
			break;
		}
		x += dx;
		y += dy;
	}

	if (noSpace && board[x][y] != EMPTYCELL) {
		return false;
	}

	dx = -dx;
	dy = -dy;

	while (x + dx < board.size() && y + dy < board.size() && x + dx >= 0 && y + dy >= 0 &&
		board[x + dx][y + dy] != OUTOFMAP) {

		board[x][y] = board[x + dx][y + dy];
		x += dx;
		y += dy;
	}

	board[x][y] = EMPTYCELL;

	return true;
}

bool Gipf::pushLine(std::pair<int, int>& pushSource, std::pair<int, int>& field) {

	// vector of push of the line attached to the field
	int dx = 0;
	int dy = 0;
	int x = field.first, y = field.second;
	bool noSpace = true;

	if (board[x][y] == EMPTYCELL) {
		return true;
	}

	std::pair<int, int> translations = getPushVector(pushSource, field);
	dx = translations.first;
	dy = translations.second;

	while (x + dx < board.size() && y + dy < board.size() && x + dx >= 0 && y + dy >= 0 &&
		board[x + dx][y + dy] != OUTOFMAP) {

		if (board[x][y] == EMPTYCELL) {
			noSpace = false;
			break;
		}
		x += dx;
		y += dy;
	}

	if (noSpace && board[x][y] != EMPTYCELL) {
		return false;
	}

	dx = -dx;
	dy = -dy;

	while (x + dx < board.size() && y + dy < board.size() && x + dx >= 0 && y + dy >= 0 &&
		board[x + dx][y + dy] != OUTOFMAP) {

		board[x][y] = board[x + dx][y + dy];
		x += dx;
		y += dy;
	}

	board[x][y] = EMPTYCELL;

	return true;
}

void Gipf::loadGameBoard() {
	int set_size, set_pawnsCollect;
	int set_whiteMaxPawns, set_blackMaxPawns;
	int set_whitePawns, set_blackPawns;
	char set_turn;
	cin >> set_size >> set_pawnsCollect;
	cin >> set_whiteMaxPawns >> set_blackMaxPawns;

	// needed when using premade maps
	// cin >> set_whitePawns >> set_blackPawns >> set_turn;
	// set_turn = set_turn == WHITEPAWN ? WHITETURN : BLACKTURN;

	set_turn = WHITETURN;
	set_whitePawns = 0;
	set_blackPawns = 0;



	Gipf new_gipf(createBoard(set_size), set_size, set_pawnsCollect,
		set_whiteMaxPawns, set_blackMaxPawns, set_whitePawns, set_blackPawns, set_turn);

	*this = new_gipf;
	boardEmpty = !loadBoardState();
}

int Gipf::translateCommand(std::string command) {
	if (command == "LOAD_GAME_BOARD") {
		return LOAD_GAME_BOARD;
	}
	else if (command == "PRINT_GAME_BOARD") {
		return PRINT_GAME_BOARD;
	}
	else if (command == "P") {
		return PRINT_GAME_BOARD;
	}
	else if (command == "p") {
		return PRINT_GAME_BOARD;
	}
	else if (command == "d") {
		return DO_MOVE;
	}
	else if (command == "DO_MOVE") {
		return DO_MOVE;
	}
	else if (command == "PRINT_GAME_STATE") {
		return PRINT_GAME_STATE;
	}
	else if (command == "IS_GAME_OVER") {
		return IS_GAME_OVER;
	}
	else if (command == "GEN_ALL_POS_MOV" || command == "gen") {
		return GEN_ALL_POS_MOV;
	}
	else if (command == "GEN_ALL_POS_MOV_EXT") {
		return GEN_ALL_POS_MOV_EXT;
	}
	else if (command == "GEN_ALL_POS_MOV_NUM" || command == "gennum") {
		return GEN_ALL_POS_MOV_NUM;
	}
	else if (command == "GEN_ALL_POS_MOV_EXT_NUM") {
		return GEN_ALL_POS_MOV_EXT_NUM;
	}
	else if (command == "WINNING_SEQUENCE_EXIST") {
		return WINNING_SEQUENCE_EXIST;
	}
	else if (command == "SOLVE_GAME_STATE") {
		return SOLVE_GAME_STATE;
	}
	else {
		return INVALID_COMMAND;
	}
}

vector<vector<char>> Gipf::getBoard() const {
	return board;
}

char Gipf::currentColor() const {
	return turn == WHITETURN ? WHITEPAWN : BLACKPAWN;
}

char Gipf::nextColor() const {
	return turn == WHITETURN ? BLACKPAWN : WHITEPAWN;
}

const GipfPlayer* Gipf::currentPlayer() const {
	return turn == WHITE ? &white : &black;
}

GipfPlayer* Gipf::currentPlayer() {
	return turn == WHITE ? &white : &black;
}

const GipfPlayer* Gipf::nextPlayer() const {
	return turn == WHITE ? &black : &white;
}

int Gipf::getPawnsCollect() {
	return pawnsCollect;
}

GipfPointsManager* Gipf::getManager() {
	return &manager;
}

char Gipf::getColor(pair<int, int> slot) {
	return board[slot.first][slot.second];
}

std::pair<std::string, int> Gipf::separateIndex(std::string& index) {
	std::string letter, number;
	int i = 0;

	// create two parts
	while (index.at(i) > '9') {
		letter += index.at(i);
		i++;
	}

	while (i < index.size()) {
		number += index.at(i);
		i++;
	}

	int int_number = std::stoi(number);

	return std::pair<std::string, int>(letter, int_number);
}

std::pair<int, int> Gipf::getPushVector(std::string& pushSource, std::string& field) {
	int dx = 0, dy = 0, letterRatio, numberRatio;

	std::pair<std::string, int> source = separateIndex(pushSource);
	std::pair<std::string, int> field_s = separateIndex(field);
	int sourceNumber = source.second, fieldNumber = field_s.second;
	std::string sourceLetter = source.first, fieldLetter = field_s.first;

	if (sourceLetter == fieldLetter) {
		letterRatio = EQUAL;
	}
	else {
		letterRatio = sourceLetter > fieldLetter ? RIGHTSMALLER : LEFTSMALLER;
	}

	if (sourceNumber == fieldNumber) {
		numberRatio = EQUAL;
	}
	else {
		numberRatio = sourceNumber > fieldNumber ? RIGHTSMALLER : LEFTSMALLER;
	}

	switch (letterRatio) {
	case LEFTSMALLER:
		switch (numberRatio) {
		case LEFTSMALLER:
			dx = RIGHT;
			break;
		case EQUAL:
			dx = RIGHT;
			dy = DOWN;
			break;
		case RIGHTSMALLER:
			dx = RIGHT;
			dy = DOWN;
		}
		break;
	case EQUAL:
		switch (numberRatio) {
		case LEFTSMALLER:
			dy = UP;
			break;
		case EQUAL:
			dx = RIGHT;
			break;
		case RIGHTSMALLER:
			dy = DOWN;
			break;
		}
		break;
	case RIGHTSMALLER:
		switch (numberRatio) {
		case LEFTSMALLER:
			dy = UP;
			dx = LEFT;
			break;
		case EQUAL:
			dx = LEFT;
			break;
		case RIGHTSMALLER:
			dx = LEFT;
			break;
		}
		break;
	}

	return std::pair<int, int>(dx, dy);
}

std::pair<int, int> Gipf::getPushVector(const std::pair<int, int> start, const std::pair<int, int> end) {
	std::pair<int, int> direction(NONE, NONE);

	if (start.first == end.first) {
		if (start.second > end.second) {
			direction = { NONE, UP };
		}
		else {
			direction = { NONE, DOWN };
		}
	}
	else if (start.second == end.second) {
		if (start.first > end.first) {
			direction = { LEFT, NONE };
		}
		else {
			direction = { RIGHT, NONE };
		}
	}
	else {
		if (start.first > end.first && start.second > end.second) {
			direction = { LEFT, UP };
		}
		else {
			direction = { RIGHT, DOWN };
		}
	}

	if (direction.first == NONE && direction.second == NONE) {
		std::cout << "error getPushVector gipf\n";
	}

	return direction;
}

int Gipf::countRowsInColumn(int index) {
	return (int)board.size() - abs(size - 1 - index);
}

bool Gipf::insideBoard(vector<vector<char>>& checkedBoard, std::pair<int, int> coords) {
	if (coords.first < 0 || coords.first > checkedBoard.size() - 1) {
		return false;
	}
	if (coords.second < 0 || coords.second > checkedBoard.size() - 1) {
		return false;
	}
	if (checkedBoard[coords.first][coords.second] == OUTOFMAP) {
		return false;
	}

	return true;
}

bool Gipf::insideBoard(const vector<vector<char>>& checkedBoard, int col, int row) {
	if (col < 0 || col > checkedBoard.size() - 1) {
		return false;
	}
	if (row < 0 || row > checkedBoard.size() - 1) {
		return false;
	}
	if (checkedBoard[col][row] == OUTOFMAP) {
		return false;
	}

	return true;
}

int Gipf::countRowStart(int index) {
	if (index < size) {
		return 0;
	}
	else {
		return index - size + 1;
	}
}

std::pair<int, int> Gipf::getChainsHorizontally() {
	int whiteCount = 0;
	int blackCount = 0;
	int totalWhiteRows = 0;
	int totalBlackRows = 0;
	int mode = WHITE;

	for (int row = 0; row < board.size(); row++) {
		for (int col = 0; col < board.size(); col++) {
			handleChainsCountingAtLoading(col, row, whiteCount, blackCount,
				totalWhiteRows, totalBlackRows, mode);
		}

		if (whiteCount >= pawnsCollect) {
			totalWhiteRows++;
		}
		else if (blackCount >= pawnsCollect) {
			totalBlackRows++;
		}
		whiteCount = 0;
		blackCount = 0;
	}

	return std::pair<int, int>(totalWhiteRows, totalBlackRows);
}

std::pair<int, int> Gipf::getChainsVertically() {
	int whiteCount = 0;
	int blackCount = 0;
	int totalWhiteRows = 0;
	int totalBlackRows = 0;
	int mode = WHITE;

	for (int col = 0; col < board.size(); col++) {
		for (int row = 0; row < board.size(); row++) {
			handleChainsCountingAtLoading(col, row, whiteCount, blackCount,
				totalWhiteRows, totalBlackRows, mode);
		}

		if (whiteCount >= pawnsCollect) {
			totalWhiteRows++;
		}
		else if (blackCount >= pawnsCollect) {
			totalBlackRows++;
		}
		whiteCount = 0;
		blackCount = 0;
	}

	return std::pair<int, int>(totalWhiteRows, totalBlackRows);
}

std::pair<int, int> Gipf::getChainsDiagonally() {
	int whiteCount = 0;
	int blackCount = 0;
	int totalWhiteRows = 0;
	int totalBlackRows = 0;
	int mode = WHITE;

	int col, row;

	for (int rowStart = 0; rowStart < size; rowStart++) {
		row = rowStart;
		col = 0;
		for (int i = 0; i < size * 2 - 1 - rowStart; i++) {
			handleChainsCountingAtLoading(col, row, whiteCount, blackCount,
				totalWhiteRows, totalBlackRows, mode);

			row++;
			col++;
		}
		if (whiteCount >= pawnsCollect) {
			totalWhiteRows++;
		}
		else if (blackCount >= pawnsCollect) {
			totalBlackRows++;
		}
		whiteCount = 0;
		blackCount = 0;
	}

	for (int colStart = 1; colStart < size; colStart++) {
		row = 0;
		col = colStart;
		for (int i = 0; i < size * 2 - 1 - colStart; i++) {
			handleChainsCountingAtLoading(col, row, whiteCount, blackCount,
				totalWhiteRows, totalBlackRows, mode);

			row++;
			col++;
		}
		if (whiteCount >= pawnsCollect) {
			totalWhiteRows++;
		}
		else if (blackCount >= pawnsCollect) {
			totalBlackRows++;
		}
		whiteCount = 0;
		blackCount = 0;
	}

	return std::pair<int, int>(totalWhiteRows, totalBlackRows);
}

void Gipf::handleChainsCountingAtLoading(int& col, int& row, int& whiteCount, int& blackCount,
	int& totalWhiteRows, int& totalBlackRows, int& mode) {

	char curField = board[col][row];
	switch (mode) {
	case WHITE:
		if (curField == WHITEPAWN) {
			whiteCount++;
			break;
		}

		if (whiteCount >= pawnsCollect) {
			totalWhiteRows++;
		}

		whiteCount = 0;
		
		if (curField == BLACKPAWN) {
			blackCount++;
			mode = BLACK;
		}
		break;
	case BLACK:
		if (curField == BLACKPAWN) {
			blackCount++;
			break;
		}

		if (blackCount >= pawnsCollect) {
			totalBlackRows++;
		}

		blackCount = 0;

		if (curField == WHITEPAWN) {
			whiteCount++;
			mode = WHITE;
		}
		break;
	}
}

void Gipf::getMove(std::string& move, char& turn, std::string& start, std::string& end) {
	char sign;
	cin >> move;
	sign = getchar();
	if (sign != ' ') {
		return;
	}
	turn = getchar();

	sign = getchar();
	sign = getchar();
	if (sign != ' ') {
		return;
	}
	cin >> start;

	sign = getchar();
	if (sign != ' ') {
		return;
	}
	cin >> end;
}

bool Gipf::chainHasIntersection(const Chain& chain, vector<vector<Chain>>& intersectingChains) {
	for (auto& intersection : intersectingChains) {
		for (auto& curChain : intersection) {
			if (curChain == chain) {
				return true;
			}
		}
	}
	return false;
}

Gipf::~Gipf() {

}
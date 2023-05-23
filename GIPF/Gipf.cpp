#include "Gipf.h"
#include "Defines.h"

using std::cin;

Gipf::Gipf(vector<vector<char>> board, int size, int pawnsCollect, int whiteMaxPawns, int blackMaxPawns,
	int whitePawns, int blackPawns, int turn) 
	: black(blackPawns, blackMaxPawns, BLACKPAWN), white(whitePawns, whiteMaxPawns, WHITEPAWN) {
	this->board = board;
	this->size = size;
	this->running = true;
	this->turn = turn;
	this->pawnsCollect = pawnsCollect;
}

void Gipf::operator=(Gipf& new_gipf) {
	this->board = new_gipf.board;
	this->size = new_gipf.size;
	this->running = new_gipf.running;
	this->turn = new_gipf.turn;
	this->pawnsCollect = new_gipf.pawnsCollect;
	this->white = new_gipf.white;
	this->black = new_gipf.black;
}

void Gipf::print() {

	/*for (int k = 0; k < abs(size - 1) + 1; k++) {
		std::cout << " ";
	}

	for (int i = 0; i < size + 1; i++) {
		std::cout << "+ ";
	}

	std::cout << "\n";*/

	for (int i = 0; i < board.size(); i++) {

		for (int k = 0; k < abs(size - 1 - i); k++) {
			std::cout << " ";
		}

		// std::cout << "+ ";

		for (int j = 0; j < board.size(); j++) {
			if (board[j][i] != OUTOFMAP) {
				std::cout << board[j][i] << " ";
			}
		}
		// std::cout << BORDER;
		std::cout << "\n";
	}

	/*for (int k = 0; k < abs(size - 1) + 1; k++) {
		std::cout << " ";
	}

	for (int i = 0; i < size + 1; i++) {
		std::cout << "+ ";
	}*/

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

			break;
		case GEN_ALL_POS_MOV:

			break;
		case GEN_ALL_POS_MOV_EXT:

			break;
		case GEN_ALL_POS_MOV_NUM:

			break;
		case GEN_ALL_POS_MOV_EXT_NUM:

			break;
		case WINNING_SEQUENCE_EXIST:

			break;
		case SOLVE_GAME_STATE:

			break;
		default:

			break;
	}
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

	if (whiteCount != white.getUsedPawns()) {
		std::cout << "WRONG_WHITE_PAWNS_NUMBER\n";
	}
	else if (blackCount != black.getUsedPawns()) {
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

	std::cout << "MOVE_COMMITTED";

	player->usePawn();
	endTurn();
}

void Gipf::endTurn() {
	turn = turn == WHITETURN ? BLACKTURN : WHITETURN;
}

int Gipf::checkIfWrongIndex(std::pair<int, int>& source_p, std::pair<int, int>& field_p) {
	if (source_p.first < -1) {
		return BAD_MOVE_FIRST_IS_WRONG_INDEX;
	}
	if (source_p.second < -1) {
		return BAD_MOVE_FIRST_IS_WRONG_INDEX;
	}
	if ((int)source_p.first > (int)board.size() + 1) {
		return BAD_MOVE_FIRST_IS_WRONG_INDEX;
	}
	int rows = 0;
	if (source_p.first < size) {
		rows = (size + 1) + source_p.first + 1;
	}
	else {
		rows = (size + 1) + ((int)board.size() - source_p.first);
	}
	if (source_p.second >= rows) {
		return BAD_MOVE_FIRST_IS_WRONG_INDEX;
	}

	if (field_p.first < -1) {
		return BAD_MOVE_SECOND_IS_WRONG_INDEX;
	}
	if (field_p.second < -1) {
		return BAD_MOVE_SECOND_IS_WRONG_INDEX;
	}
	if ((int)field_p.first > (int)board.size() + 1) {
		return BAD_MOVE_SECOND_IS_WRONG_INDEX;
	}
	rows = 0;
	if (field_p.first < size) {
		rows = (size + 1) + field_p.first + 1;
	}
	else {
		rows = (size + 1) + ((int)board.size() - field_p.first);
	}
	if (field_p.second >= rows) {
		return BAD_MOVE_SECOND_IS_WRONG_INDEX;
	}

	return NOERRORS;
}

int Gipf::checkIfUnknownDirection(std::pair<int,int>& source_p, std::pair<int, int>& field_p) {
	if (abs(source_p.first - field_p.first) > 1) {
		return UNKNOWN_MOVE_DIRECTION;
	}
	if (abs(source_p.second - field_p.second) > 1) {
		return UNKNOWN_MOVE_DIRECTION;
	}

	return NOERRORS;
}

int Gipf::moveValid(std::string& source, std::string& field) {

	std::pair<int, int> source_p = getWhereToPut(source);
	std::pair<int, int> field_p = getWhereToPut(field);
	int wrongIndex = checkIfWrongIndex(source_p, field_p);
	if (wrongIndex != NOERRORS) {
		return wrongIndex;
	}

	int unknownDirection = checkIfUnknownDirection(source_p, field_p);
	if (unknownDirection != NOERRORS) {
		return unknownDirection;
	}
}

void Gipf::doMove() {
	std::string move, pushSource, field;
	cin >> move;

	loadMoveSegments(move, pushSource, field);

	std::cout << pushSource << " " << field << "\n";

	std::pair<int, int> coords = getWhereToPut(field);
	int x = coords.first;
	int y = coords.second;

	int valid = moveValid(pushSource, field);
	if (valid != 0) {
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
		}
	}

	if (board[x][y] != EMPTYCELL) {
		bool success = pushLine(pushSource, field, coords);

		if (success) {
			putPawn(x, y);
		}
	}
	else {
		putPawn(x, y);
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

std::pair<int, int> Gipf::getWhereToPut(std::string& index) {
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
	/*for (int i = 0; i < board.size(); i++) {
		if (i == index1) {
			break;
		}

		rows += (i < size ? 1 : -1);
	}*/

	rows = (int)board.size() - abs(size - 1 - index1);

	index2 = rows - number + 1;

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
}

void Gipf::loadGameBoard() {
	int set_size, set_pawnsCollect;
	int set_whiteMaxPawns, set_blackMaxPawns;
	int set_whitePawns, set_blackPawns;
	char set_turn;
	cin >> set_size >> set_pawnsCollect;
	cin >> set_whiteMaxPawns >> set_blackMaxPawns;
	cin >> set_whitePawns >> set_blackPawns >> set_turn;
	set_turn = set_turn == WHITEPAWN ? WHITETURN : BLACKTURN;

	Gipf new_gipf(createBoard(set_size), set_size, set_pawnsCollect, 
		set_whiteMaxPawns, set_blackMaxPawns, set_whitePawns, set_blackPawns, set_turn);

	*this = new_gipf;
	loadBoardState();
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
	else if (command == "DO_MOVE") {
		return DO_MOVE;
	}
	else if (command == "PRINT_GAME_STATE") {
		return PRINT_GAME_STATE;
	}
	else if (command == "GEN_ALL_POS_MOV") {
		return GEN_ALL_POS_MOV;
	}
	else if (command == "GEN_ALL_POS_MOV_EXT") {
		return GEN_ALL_POS_MOV_EXT;
	}
	else if (command == "GEN_ALL_POS_MOV_NUM") {
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

char Gipf::currentColor() {
	return turn == WHITETURN ? WHITEPAWN : BLACKPAWN;
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
			dx = 1;
			break;
		case EQUAL:
			dx = 1;
			dy = 1;
			break;
		case RIGHTSMALLER:
			std::cout << "ERROR GETPUSHVECTOR - LEFTSMALLER -> RIGHTSMALLER\n";
		}
		break;
	case EQUAL:
		switch (numberRatio) {
		case LEFTSMALLER:
			dy = -1;
			break;
		case EQUAL:
			std::cout << "ERROR GETPUSHVECTOR - EQUAL -> EQUAL\n";
			break;
		case RIGHTSMALLER:
			dy = 1;
			break;
		}
		break;
	case RIGHTSMALLER:
		switch (numberRatio) {
		case LEFTSMALLER:
			std::cout << "ERROR GETPUSHVECTOR - RIGHTSMALLER -> LEFTSMALLER\n";
			break;
		case EQUAL:
			dx = -1;
			break;
		case RIGHTSMALLER:
			dy = -1;
			dx = -1;
			break;
		}
		break;
	}

	return std::pair<int, int>(dx, dy);
}


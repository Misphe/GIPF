#include "Controller.h"
#include "Defines.h"
#include <iostream>

Controller::Controller(Game* new_game) : game(new_game) {}

std::string Controller::getCommand() {
    std::string command;
    if (std::cin >> command) {
        return command;
    }
    else if (std::cin.eof()) {
        exit(0);
    }
    return command;
}

void Controller::passCommand() {
    std::string raw_command = getCommand();
    int command = game->translateCommand(raw_command);
    game->executeCommand(command);
}

bool Controller::gameIsRunning() {
    return game->isRunning();
}

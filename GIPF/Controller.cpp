#include "Controller.h"
#include "Defines.h"
#include <iostream>

Controller::Controller(Game2D* new_game) : game(new_game) {}

std::string Controller::getCommand() {
    std::string command;
    if (std::cin >> command) {
        return command;
    }
    else if (std::cin.eof()) {
        return "end";
    }
    return command;
}

void Controller::passCommand() {
    std::string raw_command = getCommand();
    if (raw_command == "end") {
        game->end();
        return;
    }

    int command = game->translateCommand(raw_command);
    game->executeCommand(command);
}

bool Controller::gameIsRunning() {
    return game->isRunning();
}

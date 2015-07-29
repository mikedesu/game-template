#include "Game.h"
Game::Game() {
    name = "game";
}
std::string Game::getName() { 
    return name;
}
void Game::setName(std::string s) {
    name = s;
}


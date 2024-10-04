#ifndef GAME_LOGIC_HPP
#define GAME_LOGIC_HPP

#include <vector>
#include <string>
#include "Pet.hpp"

class GameLogic {
private:
    int test = 0; //for testing
    int compteur = 0; //for testing

    // variable for display
    size_t numberDrawnLines = 0;

    // variable for menu
    std::vector<std::string> menus = {"feed", "clean", "quit"};
    size_t activeMenuIndex = 0;

    // variable for animation state
    std::vector<std::string> states = {"1", "2", "3", "4", "5", "6"};
	size_t current_state = 0;

    void processInput(char input);
    void updateGame(Pet *pet);
    void renderGame(Pet *pet);

public:
    GameLogic();
    void run(Pet *pet);
};

#endif // GAME_LOGIC_HPP
#ifndef GAME_LOGIC_HPP
#define GAME_LOGIC_HPP

#include <vector>
#include <string>
#include <csignal>
#include "Pet.hpp"

class GameLogic {
private:
    bool gameRunning = true;
    static GameLogic* instance;

    // variable for display
    size_t numberDrawnLines = 0;

    // variable for menu
    std::vector<std::string> menus = {"feed", "clean", "toilet", "quit"};
    size_t activeMenuIndex = 0;

    // variable for animation state
    std::vector<std::string> states = {"1", "2", "3", "4", "5", "6"};
	size_t current_state = 0;

    static void signal_handler(int signal);

    void processInput(char input, Pet *pet);
    void renderGame(Pet *pet);

public:
    GameLogic();
    ~GameLogic();
    void run(Pet *pet);
};

#endif // GAME_LOGIC_HPP
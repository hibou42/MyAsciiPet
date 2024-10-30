#ifndef GAME_LOGIC_HPP
#define GAME_LOGIC_HPP

#include <csignal>
#include <cmath>
#include "Pet.hpp"
#include "display_manager.hpp"

class GameLogic {
private:
    bool gameRunning = true;
    static GameLogic* instance;

    DisplayManager displayManager;

    static void signal_handler(int signal);
    void processInput(char input, Pet *pet);

public:
    GameLogic();
    ~GameLogic();
    void run(Pet *pet);
};

#endif // GAME_LOGIC_HPP
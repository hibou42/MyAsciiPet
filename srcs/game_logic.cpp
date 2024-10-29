#include "../include/game_logic.hpp"
#include "../include/input_handler.hpp"
#include "../include/Pet.hpp"
#include <iostream>
#include <chrono>
#include <thread>
#include <fstream>
#include <ctime>
#include <cstdlib>

GameLogic* GameLogic::instance = nullptr;

GameLogic::GameLogic() {
    instance = this;
}

GameLogic::~GameLogic() {
    instance = nullptr;
}

void GameLogic::signal_handler(int signal) {
    if (signal == SIGINT && instance) {
        std::cout <<"Ctrl+C trigger -> stop gameloop" << std::endl;
        instance->gameRunning = false;
        std::cout <<"Your game has been not saved" << std::endl;
        resetTerminal();
        exit(0);
    }
}

void GameLogic::run(Pet *pet) {
    std::signal(SIGINT, GameLogic::signal_handler);
    const std::chrono::milliseconds loop_duration(500); // 1000ms = 1 seconde

    while (this->gameRunning == true) {
        auto debut = std::chrono::steady_clock::now();
        char lastKey = 0;

        do {
            char key = readKeyNonBlocking();
            if (key != 0) {
                lastKey = key;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        } while (std::chrono::steady_clock::now() - debut < loop_duration);

        processInput(lastKey, pet);
        pet->update_stats();
        displayManager.render(pet);
    }
}

// reminder : C = rightArrow / D = leftArrow
void GameLogic::processInput(char input, Pet *pet) {
    if (input == 'C') {
        displayManager.nextMenuIndex();
    } else if (input == 'D') {
        displayManager.previousMenuIndex();
    } else if (input == '\n') {
        switch (displayManager.getActiveMenuIndex()) {
            case 0:
                pet->feed();
                break;
            case 1:
                pet->clean();
                break;
            case 2:
                pet->use_toilet();
                break;
            case 3: // Quit
                this->gameRunning = false;
            default:
                // std::cout << "Action non reconnue." << std::endl;
                break;
        }
    }
}
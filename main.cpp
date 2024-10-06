#include <iostream>
#include <string>
#include <map>
#include <ctime>
#include <chrono>
#include <thread>
#include <fstream>
#include <nlohmann/json.hpp>
#include <filesystem>
#include <csignal>
#include <atomic>

#include "include/input_handler.hpp"
#include "include/game_logic.hpp"
#include "include/PetManager.hpp"

int main(int argc, char* argv[]) {
    PetManager manager;
    configureTerminal();
    GameLogic game;
    game.run(manager.get_active_pet());

    resetTerminal();
    return 0;
}
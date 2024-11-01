#include "../include/display_manager.hpp"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>

DisplayManager::DisplayManager() 
    : numberDrawnLines(0), 
      menus({"feed", "clean", "toilet", "quit"}),
      activeMenuIndex(0),
      states({"1", "2", "3", "4", "5", "6"}),
      current_state(0) {}

void DisplayManager::clearScreen() {
    while (this->numberDrawnLines > 0) { 
        std::cout << "\033[1A"; // Move the cursor up one line
        std::cout << "\033[2K"; // Erase the entire line where the cursor is currently positioned
        this->numberDrawnLines--;
    }
}

void DisplayManager::displayPetInfo(const Pet* pet) {
    std::cout << "My ASCII Pet: " << pet->get_name() << std::endl;
    std::cout << "LVL " << pet->get_level() << " ";
    std::cout << pet->get_xp() << "/" << pet->getXpForNextLevel() << "xp" << std::endl;
    this->numberDrawnLines += 2;
}

void DisplayManager::displayAnimation(const std::string& petName) {
    std::string file_path = "./assets/owl" + this->states[this->current_state];
    std::ifstream file(file_path);
    if (!file.is_open()) {
        std::cerr << "Could not open the file: " << file_path << std::endl;
        return;
    }
    
    std::srand(std::time(nullptr));
    char c;
    bool is_new_line = true;
    while (file.get(c)) {
        if (is_new_line) {
            for (size_t i = 0; i < this->current_state / 2; ++i) {
                std::cout << ' ';
            }
            is_new_line = false;
        }
        if (c == 'v') {
            char replacement_chars[] = {'v', 'o', 'u', 'x'};
            c = replacement_chars[std::rand() % 4];
        }
        std::cout << c;
        if (c == '\n') {
            is_new_line = true;
            this->numberDrawnLines++;
        }
    }
    std::cout << std::endl;
    this->numberDrawnLines++;
    file.close();
    this->current_state = (this->current_state + 1) % this->states.size();
}

void DisplayManager::displayStats(const Pet* pet) {
    std::cout << "Happiness: " << pet->get_happiness() << std::endl;
    std::cout << "Hunger: " << pet->get_hunger() << std::endl;
    std::cout << "Cleanliness: " << pet->get_cleanliness() << std::endl;
    std::cout << "Toilet: " << pet->get_toilet() << std::endl;
    this->numberDrawnLines += 4;
}

void DisplayManager::displayAction(const Pet *pet) {
    std::time_t current_time = std::time(nullptr);
    std::time_t diff_time = current_time - pet->get_last_action_time();
    if (pet->get_last_action().empty() || diff_time >= 2) {
        std::cout << "                                  " << std::endl;
        this->numberDrawnLines++;
    }
    else {
        std::cout << pet->get_last_action() << std::endl;
        this->numberDrawnLines++;
    }
}

void DisplayManager::displayMenu() {
    std::cout << "__________________________________" << std::endl << "| ";
    this->numberDrawnLines++;
    for (size_t i = 0; i < this->menus.size(); ++i) {
        if (i == this->activeMenuIndex) {
            std::cout << "|" << this->menus[i] << "| ";
        } else {
            std::cout << " " << this->menus[i] << "  ";
        }
    }
    std::cout << "|" << std::endl;
    this->numberDrawnLines++;
}

void DisplayManager::render(const Pet* pet) {
    clearScreen();
    displayPetInfo(pet);
    displayAnimation(pet->get_name());
    displayStats(pet);
    displayAction(pet);
    displayMenu();
}

void DisplayManager::setActiveMenuIndex(size_t index) {
    this->activeMenuIndex = index % this->menus.size();
}

size_t DisplayManager::getActiveMenuIndex() const {
    return this->activeMenuIndex;
}

void DisplayManager::nextMenuIndex() {
    this->activeMenuIndex = (this->activeMenuIndex + 1) % this->menus.size();
}

void DisplayManager::previousMenuIndex() {
    if (this->activeMenuIndex == 0) {
        this->activeMenuIndex = this->menus.size() - 1;
    } else {
        this->activeMenuIndex--;
    }
}

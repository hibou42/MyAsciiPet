#ifndef DISPLAY_MANAGER_HPP
#define DISPLAY_MANAGER_HPP

#include <string>
#include <vector>
#include "Pet.hpp"

class DisplayManager {
private:
    size_t numberDrawnLines;
    std::vector<std::string> menus;
    size_t activeMenuIndex;
    std::vector<std::string> states;
    size_t current_state;

public:
    DisplayManager();
    
    void clearScreen();
    void displayPetName(const std::string& name);
    void displayAnimation(const std::string& petName);
    void displayStats(const Pet* pet);
    void displayMenu();
    void render(const Pet* pet);
    
    void setActiveMenuIndex(size_t index);
    size_t getActiveMenuIndex() const;
    void nextMenuIndex();
    void previousMenuIndex();
};

#endif // DISPLAY_MANAGER_HPP
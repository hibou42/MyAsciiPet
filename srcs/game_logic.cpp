#include "../include/game_logic.hpp"
#include "../include/input_handler.hpp"
#include "../include/Pet.hpp"
#include <iostream>
#include <chrono>
#include <thread>
#include <fstream>
#include <ctime>
#include <cstdlib>

GameLogic::GameLogic() {}

void GameLogic::run(Pet *pet) {
    while (true) {
        auto debut = std::chrono::steady_clock::now();
        char lastKey = 0;

        do {
            char key = readKeyNonBlocking();
            if (key != 0) {
                lastKey = key;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        } while (std::chrono::duration_cast<std::chrono::seconds>(
                     std::chrono::steady_clock::now() - debut)
                     .count() < 1);
        processInput(lastKey);
        updateGame(pet);
        renderGame(pet);
    }
}

// reminder : C = rightArrow / D = leftArrow
void GameLogic::processInput(char input) {
    if (input == 'q') {
        std::cout << "Sortie du programme." << std::endl;
        exit(0);
    } else if (input == 'C') {
        this->activeMenuIndex = (this->activeMenuIndex + 1) % this->menus.size();
    } else if (input == 'D') {
        if (this->activeMenuIndex == 0) {
            this->activeMenuIndex = this->menus.size() - 1;
        } else {
            this->activeMenuIndex--;
        }
    } else if (input == '\n') {
        this->compteur++;
    }
}

void GameLogic::updateGame(Pet *pet) {
    pet->update_stats();
    test++;
}

void GameLogic::renderGame(Pet *pet) {
    // cleaning
    while (this->numberDrawnLines > 0) {
        std::cout << "\033[1A";
        this->numberDrawnLines--;
    }

    // start drawing
    std::cout << "My ASCII Pet: WiP" << std::endl;
    this->numberDrawnLines++;

    //animation drawing
    std::string file_path = "./ascii_art/owl" + this->states[this->current_state];
    std::ifstream file(file_path);
	if (!file.is_open()) {
		std::cerr << "Could not open the file: " << file_path << std::endl;
		return;
	}
	std::srand(std::time(nullptr)); // Seed for randomness
	char c;
	bool is_new_line = true;
	int space_counter = 0;
	bool increasing = true;
	while (file.get(c)) {
		if (is_new_line) {
			// Print spaces according to the state value at the start of each line
			for (size_t i = 0; i < this->current_state / 2; ++i) {
				std::cout << ' ';
			}
			is_new_line = false; // Reset the flag
		}
		if (c == 'v') {
			char replacement_chars[] = {'v', 'o', 'u', 'x'};
			c = replacement_chars[std::rand() % 4]; // Corrected the array size to 4
		}
		std::cout << c;
		if (c == '\n') {
			is_new_line = true; // Set the flag if we encounter a newline character
            this->numberDrawnLines++;
		}
	}
	std::cout << std::endl;
    this->numberDrawnLines++;
	file.close();
    this->current_state = (this->current_state + 1) % this->states.size();

    // stats drawing
    this->numberDrawnLines += pet->display_stats();

    // menu drawing
    for (size_t i = 0; i < this->menus.size(); ++i) {
            if (i == this->activeMenuIndex) {
                std::cout << "|" << this->menus[i] << "| ";
            } else {
                std::cout << " " << this->menus[i] << "  ";
            }
        }
        std::cout << std::endl;
        this->numberDrawnLines++;
    std::cout << test << " Compteur : " << compteur << std::endl;
    this->numberDrawnLines++;
}
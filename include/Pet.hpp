#ifndef PET_HPP
# define PET_HPP

#include <string>
#include <iostream>
#include <ctime>
#include <algorithm>

class Pet {
private:
	std::string name;
	double hunger;
	double happiness;
	double cleanliness;
	double toilet;
	std::time_t last_update;
	std::string last_action;
	std::time_t last_action_time;

public:
	Pet(std::string name = "Default") : 
		name(name), hunger(1000), happiness(500), cleanliness(1000), toilet(0) {
		last_update = std::time(nullptr);
	}

	void update_stats() {
		std::time_t current_time = std::time(nullptr);
		
		while (last_update < current_time) {
			// Mise à jour pour une seconde
			hunger = std::max(0.0, hunger - 0.5);
			cleanliness = std::max(0.0, cleanliness - 0.2);
			toilet = std::min(1000.0, toilet + 0.4);
			
			if (hunger > 500 && cleanliness > 500 && toilet < 500) {
				happiness = std::min(1000.0, happiness + 0.3);
			} else {
				happiness = std::max(0.0, happiness - 0.3);
			}
			last_update++;
		}
	}

	void feed() {
		hunger = std::min(1000.0, hunger + 1000);
		last_action = "Vous avez nourri " + name + "!";
		last_action_time = std::time(nullptr);
	}

	void clean() {
		cleanliness = std::min(1000.0, cleanliness + 1000);
		last_action = "Vous avez lavé " + name + "!";
		last_action_time = std::time(nullptr);
	}

	void use_toilet() {
		if (toilet > 300) {
			toilet = std::max(0.0, toilet - 1000);
			cleanliness = std::max(0.0, cleanliness - 200);
			last_action = name + " a utilisé les toilettes!";
			last_action_time = std::time(nullptr);
		} else {
			last_action = name + " n'a pas besoin d'aller aux toilettes pour le moment.";
			last_action_time = std::time(nullptr);
		}
	}

	// Getters
	std::string get_name() const { return name; }
	double get_hunger() const { return hunger; }
	double get_happiness() const { return happiness; }
	double get_cleanliness() const { return cleanliness; }
	double get_toilet() const { return toilet; }
	std::time_t get_last_update() const { return last_update; }
	std::string get_last_action() const {return last_action; }
	std::time_t get_last_action_time() const {return last_action_time; }

	// Setters
	void set_hunger(double value) { hunger = value; }
	void set_happiness(double value) { happiness = value; }
	void set_cleanliness(double value) { cleanliness = value; }
	void set_toilet(double value) { toilet = value; }
	void set_last_update(std::time_t value) { last_update = value; }
	void set_last_action (std::string value) { last_action = value; }
	void set_last_action_time (std::time_t value) { last_action_time = value; }
};

#endif
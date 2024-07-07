#ifndef PET_HPP
# define PET_HPP

class Pet {
private:
	std::string name;
	double hunger;
	double happiness;
	double cleanliness;
	double bathroom;
	std::time_t last_update;

public:
	Pet(std::string name = "Default") : 
		name(name), hunger(1000), happiness(500), cleanliness(1000), bathroom(0) {
		last_update = std::time(nullptr);
	}

	void update_stats() {
		std::time_t current_time = std::time(nullptr);
		double elapsed_time = difftime(current_time, last_update);
		
		hunger = std::max(0.0, hunger - elapsed_time * 0.5);
		cleanliness = std::max(0.0, cleanliness - elapsed_time * 0.2);
		bathroom = std::min(1000.0, bathroom + elapsed_time * 0.4);
		if (hunger > 500 && cleanliness > 500 && bathroom < 500)
			happiness = std::min(1000.0, happiness + elapsed_time * 0.3);
		else
			happiness = std::max(0.0, happiness - elapsed_time * 0.3);

		last_update = current_time;
	}

	void feed() {
		hunger = std::min(1000.0, hunger + 1000);
		std::cout << "Vous avez nourri " << name << "!" << std::endl;
	}

	void clean() {
		cleanliness = std::min(1000.0, cleanliness + 1000);
		std::cout << "Vous avez nettoyé " << name << "!" << std::endl;
	}

	void use_bathroom() {
		if (bathroom > 300) {
			bathroom = std::max(0.0, bathroom - 1000);
			cleanliness = std::max(0.0, cleanliness - 200);
			std::cout << name << " a utilisé les toilettes!" << std::endl;
		} else {
			std::cout << name << " n'a pas besoin d'aller aux toilettes pour le moment." << std::endl;
		}
	}

	void display_stats() {
		std::cout << "Faim: " << hunger << std::endl;
		std::cout << "Bonheur: " << happiness << std::endl;
		std::cout << "Propreté: " << cleanliness << std::endl;
		std::cout << "Toilettes: " << bathroom << std::endl;
	}

	// Getters
	std::string get_name() const { return name; }
	double get_hunger() const { return hunger; }
	double get_happiness() const { return happiness; }
	double get_cleanliness() const { return cleanliness; }
	double get_bathroom() const { return bathroom; }
	std::time_t get_last_update() const { return last_update; }

	// Setters
	void set_hunger(double value) { hunger = value; }
	void set_happiness(double value) { happiness = value; }
	void set_cleanliness(double value) { cleanliness = value; }
	void set_bathroom(double value) { bathroom = value; }
	void set_last_update(std::time_t value) { last_update = value; }
};

#endif
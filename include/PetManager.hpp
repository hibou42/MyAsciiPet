#ifndef PETMANAGER_HPP
# define PETMANAGER_HPP

using json = nlohmann::json;
namespace fs = std::filesystem;

#include "Pet.hpp"

class PetManager {
private:
	std::map<std::string, Pet> pets;
	std::string active_pet;

public:
	PetManager() {
		fs::create_directories("saves");
		std::ifstream file("saves/pet_save.json");
		
		if (!file.is_open()) {
			std::string pet_name;
			std::cout << "Welcome ! What's you pet's name ? ";
			std::getline(std::cin, pet_name);
			std::cout << "\033[1A";
			for (int i = 0; i < 34 + pet_name.size(); i++) {
				std::cout << " ";
			}
			std::cout << std::endl << "\033[1A";

			json new_save_data = {
				{"active_pet", pet_name},
				{"pets", {
					{pet_name, {
						{"name", pet_name},
						{"level", 1},
						{"xp", 0},
						{"hunger", 1000.0},
						{"happiness", 500.0},
						{"cleanliness", 1000.0},
						{"bathroom", 0.0},
						{"last_update", std::time(nullptr)}
					}}
				}}
			};

			std::ofstream new_file("saves/pet_save.json");
			new_file << new_save_data.dump(4);  // Indentation de 4 espaces pour la lisibilité
			new_file.close();
			
			// Rouvrir le fichier en mode lecture
			file.open("saves/pet_save.json");
		}

		json save_data = json::parse(file);

		for (const auto& [name, pet_data] : save_data["pets"].items()) {
			Pet pet(pet_data["name"]);
			pet.set_level(pet_data["level"]);
			pet.set_xp(pet_data["xp"]);
			pet.set_hunger(pet_data["hunger"]);
			pet.set_happiness(pet_data["happiness"]);
			pet.set_cleanliness(pet_data["cleanliness"]);
			pet.set_toilet(pet_data["bathroom"]);
			pet.set_last_update(pet_data["last_update"]);
			pets[name] = pet;
		}

		if (!save_data["active_pet"].is_null()) {
			select_pet(save_data["active_pet"], 0);
		}
	}

	~PetManager() {
		fs::create_directories("saves");
		json save_data;
		save_data["pets"] = json::object();
		save_data["active_pet"] = active_pet;

		for (const auto& [name, pet] : pets) {
			save_data["pets"][name] = {
				{"name", pet.get_name()},
				{"level", pet.get_level()},
				{"xp", pet.get_xp()},
				{"hunger", pet.get_hunger()},
				{"happiness", pet.get_happiness()},
				{"cleanliness", pet.get_cleanliness()},
				{"bathroom", pet.get_toilet()},
				{"last_update", pet.get_last_update()}
			};
		}

		std::ofstream file("saves/pet_save.json");
		file << save_data.dump(4);
		std::cout << "Your game has been saved. Good bye !!!" << std::endl;
	}

	void create_pet(const std::string& name) {
		if (pets.find(name) != pets.end()) {
			std::cout << "Un pet nommé " << name << " existe déjà." << std::endl;
			return;
		}
		pets[name] = Pet(name);
		active_pet = name;
		std::cout << "Nouveau pet '" << name << "' créé et sélectionné." << std::endl;
	}

	void select_pet(const std::string& name, int i) {
		if (pets.find(name) != pets.end()) {
			active_pet = name;
			if (i == 1)
				std::cout << "Pet '" << name << "' sélectionné." << std::endl;
		} else {
			std::cout << "Aucun pet nommé '" << name << "' trouvé." << std::endl;
		}
	}

	void list_pets() {
		if (pets.empty()) {
			std::cout << "Aucun pet disponible." << std::endl;
		} else {
			std::cout << "Pets disponibles:" << std::endl;
			for (const auto& pair : pets) {
				std::string status = (pair.first == active_pet) ? " (actif)" : "";
				std::cout << "- " << pair.first << status << std::endl;
			}
		}
	}

	Pet* get_active_pet() {
		if (!active_pet.empty()) {
			return &pets[active_pet];
		}
		return nullptr;
	}

	const std::map<std::string, Pet>& get_pets() const { return pets; }

	std::string get_active_pet_name() const { return active_pet; }

	void add_pet(const std::string& name, const Pet& pet) { pets[name] = pet; }
};

#endif
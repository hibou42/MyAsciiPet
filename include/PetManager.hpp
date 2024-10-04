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
		try {
			std::ifstream file("saves/pet_save.json");
			if (!file.is_open()) {
				throw std::runtime_error("Fichier de sauvegarde non trouvé");
			}

			json save_data = json::parse(file);

			for (const auto& [name, pet_data] : save_data["pets"].items()) {
				Pet pet(pet_data["name"]);
				pet.set_hunger(pet_data["hunger"]);
				pet.set_happiness(pet_data["happiness"]);
				pet.set_cleanliness(pet_data["cleanliness"]);
				pet.set_bathroom(pet_data["bathroom"]);
				pet.set_last_update(pet_data["last_update"]);
				pets[name] = pet;
			}

			if (!save_data["active_pet"].is_null()) {
				select_pet(save_data["active_pet"], 0);
			}
		} catch (const std::exception& e) {
			std::cout << "Erreur lors du chargement : " << e.what() << std::endl;
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
				{"hunger", pet.get_hunger()},
				{"happiness", pet.get_happiness()},
				{"cleanliness", pet.get_cleanliness()},
				{"bathroom", pet.get_bathroom()},
				{"last_update", pet.get_last_update()}
			};
		}

		std::ofstream file("saves/pet_save.json");
		file << save_data.dump(4);
		std::cout << "Jeu sauvegardé." << std::endl;
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

	void help() {
		std::cout << "Utilisez 'create', 'select', 'list'." << std::endl;
	}

	const std::map<std::string, Pet>& get_pets() const { return pets; }
	std::string get_active_pet_name() const { return active_pet; }
	void add_pet(const std::string& name, const Pet& pet) { pets[name] = pet; }
};

#endif
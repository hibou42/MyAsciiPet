#ifndef PETMANAGER_HPP
# define PETMANAGER_HPP

#include "Pet.hpp"

class PetManager {
private:
    std::map<std::string, Pet> pets;
    std::string active_pet;

public:
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
		std::cout << "Utilisez 'create', 'select', 'list', 'feed', 'clean', ou 'bathroom'." << std::endl;
	}

    const std::map<std::string, Pet>& get_pets() const { return pets; }
    std::string get_active_pet_name() const { return active_pet; }
    void add_pet(const std::string& name, const Pet& pet) { pets[name] = pet; }
};

#endif
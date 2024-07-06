#include "include/main.hpp"

using json = nlohmann::json;
namespace fs = std::filesystem;

// Forward declarations
void save_game(const PetManager& manager, const std::string& filename);

std::atomic<bool> quit(false);
PetManager* global_manager = nullptr;

void signal_handler(int signal) {
    if (signal == SIGINT) {
        std::cout << "\nReception de Ctrl+C. Sauvegarde en cours..." << std::endl;
        quit = true;
    }
}

void save_game(const PetManager& manager, const std::string& filename = "pet_save") {
    fs::create_directories("saves");
    json save_data;
    save_data["pets"] = json::object();
    save_data["active_pet"] = manager.get_active_pet_name();

    for (const auto& [name, pet] : manager.get_pets()) {
        save_data["pets"][name] = {
            {"name", pet.get_name()},
            {"hunger", pet.get_hunger()},
            {"happiness", pet.get_happiness()},
            {"cleanliness", pet.get_cleanliness()},
            {"bathroom", pet.get_bathroom()},
            {"last_update", pet.get_last_update()}
        };
    }

    std::ofstream file("saves/" + filename + ".json");
    file << save_data.dump(4);
    //std::cout << "Jeu sauvegardé." << std::endl;
}

PetManager load_game(const std::string& filename = "pet_save") {
    PetManager manager;
    try {
        std::ifstream file("saves/" + filename + ".json");
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
            manager.add_pet(name, pet);
        }

        if (!save_data["active_pet"].is_null()) {
            manager.select_pet(save_data["active_pet"], 0);
        }

        //std::cout << "Jeu chargé." << std::endl;
    } catch (const std::exception& e) {
        std::cout << "Erreur lors du chargement : " << e.what() << std::endl;
        std::cout << "Création d'un nouveau gestionnaire." << std::endl;
    }
    return manager;
}

void display_pet(const std::string& state) {
    std::map<std::string, std::vector<std::string>> pet_states = {
        {"happy", {
            "  ^_^  ",
            "( o o )",
            " ( - ) "
        }},
        {"sad", {
            "  T_T  ",
            "( o o )",
            " ( - ) "
        }},
        {"eating", {
            "  ^_^  ",
            "( o o )",
            " (U U) "
        }}
    };

    for (const auto& line : pet_states[state]) {
        std::cout << line << std::endl;
    }
}

void animate_pet(Pet* pet, PetManager& manager) {
    std::cout << "My ASCII Pet: " << pet->get_name() << std::endl;
    std::cout << "(Ctrl+C save & quit)" << std::endl << std::endl;

    std::time_t last_save = std::time(nullptr);
    std::vector<std::string> states = {"happy", "sad", "eating"};
    size_t current_state = 0;

    global_manager = &manager;
    std::signal(SIGINT, signal_handler);
    while (!quit) {
        pet->update_stats();
        display_pet(states[current_state]);
        pet->display_stats();
        
        current_state = (current_state + 1) % states.size();
        
        std::this_thread::sleep_for(std::chrono::seconds(1));
        if (!quit)
            std::cout << "\033[7A"; // Move cursor up 7 lines

        // Sauvegarde automatique toutes les 60 secondes
        if (std::difftime(std::time(nullptr), last_save) > 60) {
            save_game(manager);
            last_save = std::time(nullptr);
        }
    }
}

int main(int argc, char* argv[]) {
    PetManager manager = load_game();

    if (argc > 1) {
        std::string command = argv[1];
        if (command == "create" && argc > 2) {
            manager.create_pet(argv[2]);
        } else if (command == "select" && argc > 2) {
            manager.select_pet(argv[2], 1);
        } else if (command == "list") {
            manager.list_pets();
        } else if (command == "feed" || command == "clean" || command == "bathroom") {
            Pet* pet = manager.get_active_pet();
            if (pet) {
                if (command == "feed") pet->feed();
                else if (command == "clean") pet->clean();
                else if (command == "bathroom") pet->use_bathroom();
            } else {
                std::cout << "Aucun pet actif. Utilisez 'select' pour en choisir un." << std::endl;
            }
        } else if (command == "help") {
            manager.help();
        } else {
            std::cout << "Argument non reconnu. Utilisez ./pet help" << std::endl;
        }
    } else {
        Pet* pet = manager.get_active_pet();
        if (pet) {
            animate_pet(pet, manager);
        } else {
            std::cout << "Aucun pet actif. Créez-en un avec 'create' ou sélectionnez-en un avec 'select'." << std::endl;
        }
    }

    save_game(manager);
    return 0;
}
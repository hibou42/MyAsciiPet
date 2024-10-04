import sys
import time
import json
import os

class Pet:
    def __init__(self, name="Default"):
        self.name = name
        self.hunger = 50
        self.happiness = 50
        self.cleanliness = 50
        self.bathroom = 0
        self.last_update = time.time()

    def update_stats(self):
        current_time = time.time()
        elapsed_time = current_time - self.last_update
        
        self.hunger = round(max(0, self.hunger - elapsed_time * 0.5), 2)
        self.happiness = round(max(0, self.happiness - elapsed_time * 0.3), 2)
        self.cleanliness = round(max(0, self.cleanliness - elapsed_time * 0.2), 2)
        self.bathroom = round(min(100, self.bathroom + elapsed_time * 0.4), 2)

        self.last_update = current_time

    def feed(self):
        self.hunger = min(100, self.hunger + 20)
        self.bathroom = min(100, self.bathroom + 15)
        print(f"Vous avez nourri {self.name}!")

    def clean(self):
        self.cleanliness = min(100, self.cleanliness + 30)
        print(f"Vous avez nettoyé {self.name}!")

    def use_bathroom(self):
        if self.bathroom > 0:
            self.bathroom = max(0, self.bathroom - 50)
            self.cleanliness = max(0, self.cleanliness - 20)
            print(f"{self.name} a utilisé les toilettes!")
        else:
            print(f"{self.name} n'a pas besoin d'aller aux toilettes pour le moment.")

    def to_dict(self):
        return {
            "name": self.name,
            "hunger": self.hunger,
            "happiness": self.happiness,
            "cleanliness": self.cleanliness,
            "bathroom": self.bathroom,
            "last_update": self.last_update
        }

    @classmethod
    def from_dict(cls, data):
        pet = cls(data["name"])
        pet.hunger = data["hunger"]
        pet.happiness = data["happiness"]
        pet.cleanliness = data["cleanliness"]
        pet.bathroom = data["bathroom"]
        pet.last_update = data["last_update"]
        return pet

class PetManager:
    def __init__(self):
        self.pets = {}
        self.active_pet = None

    def create_pet(self, name):
        if name in self.pets:
            print(f"Un pet nommé {name} existe déjà.")
            return
        self.pets[name] = Pet(name)
        self.active_pet = name
        print(f"Nouveau pet '{name}' créé et sélectionné.")

    def select_pet(self, name):
        if name in self.pets:
            self.active_pet = name
            print(f"Pet '{name}' sélectionné.")
        else:
            print(f"Aucun pet nommé '{name}' trouvé.")

    def list_pets(self):
        if not self.pets:
            print("Aucun pet disponible.")
        else:
            print("Pets disponibles:")
            for name in self.pets:
                status = "(actif)" if name == self.active_pet else ""
                print(f"- {name} {status}")

    def get_active_pet(self):
        if self.active_pet:
            return self.pets[self.active_pet]
        return None

def display_pet(state):
    pet_states = {
        "happy": [
            "  ^_^  ",
            "( o o )",
            " ( - ) "
        ],
        "sad": [
            "  T_T  ",
            "( o o )",
            " ( - ) "
        ],
        "eating": [
            "  ^_^  ",
            "( o o )",
            " (U U) "
        ]
    }
    for line in pet_states[state]:
        print(line)

def save_game(manager, filename="pet_save"):
    os.makedirs("saves", exist_ok=True)
    save_data = {
        "pets": {name: pet.to_dict() for name, pet in manager.pets.items()},
        "active_pet": manager.active_pet
    }
    with open(f"saves/{filename}.json", "w") as f:
        json.dump(save_data, f)

def load_game(filename="pet_save"):
    try:
        with open(f"saves/{filename}.json", "r") as f:
            data = json.load(f)
        manager = PetManager()
        manager.pets = {name: Pet.from_dict(pet_data) for name, pet_data in data["pets"].items()}
        manager.active_pet = data["active_pet"]
        return manager
    except FileNotFoundError:
        print(f"Sauvegarde '{filename}' non trouvée. Création d'un nouveau gestionnaire.")
        return PetManager()

def animate_pet(pet, manager):
    states = ["happy", "sad", "eating"]
    current_state = 0
    print(f"My ASCII Pet: {pet.name}\n(Ctrl+C save & quit)\n")
    last_save = time.time()
    try:
        while True:
            pet.update_stats()
            display_pet(states[current_state])
            current_state = (current_state + 1) % len(states)
            print(f"\nFaim: {pet.hunger:.2f}")
            print(f"Bonheur: {pet.happiness:.2f}")
            print(f"Propreté: {pet.cleanliness:.2f}")
            print(f"Toilettes: {pet.bathroom:.2f}")
            sys.stdout.flush()
            time.sleep(1)
            sys.stdout.write("\033[K")  # Efface la ligne courante
            sys.stdout.write("\033[8F")  # Remonte de 8 lignes
            
            # Sauvegarde automatique toutes les 60 secondes
            if time.time() - last_save > 60:
                save_game(manager)
                last_save = time.time()
                
    except KeyboardInterrupt:
        save_game(manager)
        print("\n\rJeu sauvegardé. Au revoir !")

def main():
    manager = load_game()

    if len(sys.argv) > 1:
        if sys.argv[1] == "create":
            if len(sys.argv) > 2:
                manager.create_pet(sys.argv[2])
            else:
                print("Veuillez spécifier un nom pour le nouveau pet.")
        elif sys.argv[1] == "select":
            if len(sys.argv) > 2:
                manager.select_pet(sys.argv[2])
            else:
                print("Veuillez spécifier le nom du pet à sélectionner.")
        elif sys.argv[1] == "list":
            manager.list_pets()
        elif sys.argv[1] in ["feed", "clean", "bathroom"]:
            pet = manager.get_active_pet()
            if pet:
                getattr(pet, sys.argv[1])()
                save_game(manager)
            else:
                print("Aucun pet actif. Utilisez 'select' pour en choisir un.")
        else:
            print("Argument non reconnu. Utilisez 'create', 'select', 'list', 'feed', 'clean', ou 'bathroom'.")
    else:
        pet = manager.get_active_pet()
        if pet:
            animate_pet(pet, manager)
        else:
            print("Aucun pet actif. Créez-en un avec 'create' ou sélectionnez-en un avec 'select'.")
    
    save_game(manager)

if __name__ == "__main__":
    main()

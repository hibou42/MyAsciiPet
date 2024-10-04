import sys
import time


class Tamagotchi:
    def __init__(self):
        self.hunger = 50
        self.happiness = 50
        self.cleanliness = 50
        self.bathroom = 0
        self.last_update = time.time()

    def update_stats(self):
        current_time = time.time()
        elapsed_time = current_time - self.last_update
        
        # Diminuer les stats en fonction du temps écoulé
        self.hunger = max(0, self.hunger - elapsed_time * 0.5)
        self.happiness = max(0, self.happiness - elapsed_time * 0.3)
        self.cleanliness = max(0, self.cleanliness - elapsed_time * 0.2)
        self.bathroom = min(100, self.bathroom + elapsed_time * 0.4)

        self.last_update = current_time

    def feed(self):
        self.hunger = min(100, self.hunger + 20)
        self.bathroom = min(100, self.bathroom + 15)
        print("Vous avez nourri votre Tamagotchi!")

    def clean(self):
        self.cleanliness = min(100, self.cleanliness + 30)
        print("Vous avez nettoyé votre Tamagotchi!")

    def use_bathroom(self):
        if self.bathroom > 0:
            self.bathroom = max(0, self.bathroom - 50)
            self.cleanliness = max(0, self.cleanliness - 20)
            print("Votre Tamagotchi a utilisé les toilettes!")
        else:
            print("Votre Tamagotchi n'a pas besoin d'aller aux toilettes pour le moment.")

def display_tamagotchi(state):
    tamagotchi_states = {
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
    for line in tamagotchi_states[state]:
        print(line)

def animate_tamagotchi(tamagotchi):
    states = ["happy", "sad", "eating"]
    current_state = 0
    print("My ASCII Pet (Ctrl+C pour quitter)\n")

    try:
        while True:
            tamagotchi.update_stats()
            display_tamagotchi(states[current_state])
            current_state = (current_state + 1) % len(states)
            print(f"Faim: {tamagotchi.hunger:.0f}")
            print(f"Bonheur: {tamagotchi.happiness:.0f}")
            print(f"Propreté: {tamagotchi.cleanliness:.0f}")
            print(f"Toilettes: {tamagotchi.bathroom:.0f}")
            sys.stdout.flush()

            time.sleep(1)
            sys.stdout.write("\033[K")  # Efface la ligne courante
            sys.stdout.write("\033[7F")  # Remonte de 7 lignes
    except KeyboardInterrupt:
        print("\n\rAu revoir !")

def main():
    tamagotchi = Tamagotchi()

    if len(sys.argv) > 1:
        if sys.argv[1] == "feed":
            tamagotchi.feed()
        elif sys.argv[1] == "clean":
            tamagotchi.clean()
        elif sys.argv[1] == "bathroom":
            tamagotchi.use_bathroom()
        else:
            print("Argument non reconnu. Utilisez 'feed', 'clean', ou 'bathroom'.")
    else:
        animate_tamagotchi(tamagotchi)

if __name__ == "__main__":
    main()

import os
import time
import sys
import threading
from pynput import keyboard

# Variable globale pour contrôler l'état de l'écouteur
running = True

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

def on_press(key):
    global running
    try:
        if key.char == 'q':
            running = False
            return False  # Arrête le listener
    except AttributeError:
        pass

def listen_for_keys():
    with keyboard.Listener(on_press=on_press) as listener:
        listener.join()

def main():
    global running
    states = ["happy", "sad", "eating"]
    current_state = 0

    print("My ASCII Pet\n")

    # Démarrer le thread pour l'écoute des touches
    listener_thread = threading.Thread(target=listen_for_keys)
    listener_thread.start()

    while running:
        display_tamagotchi(states[current_state])
        current_state = (current_state + 1) % len(states)
        time.sleep(1)
        if running:
            sys.stdout.write("\033[F" * 3)  # Déplacer le curseur vers le haut de 3 lignes
            sys.stdout.flush()

    # Attendre que le thread de l'écouteur se termine
    listener_thread.join()
    print("\rAu revoir !")

if __name__ == "__main__":
    main()


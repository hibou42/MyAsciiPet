/*
Dans cet exemple :
Nous définissons une classe EventEmitter qui gère les événements et les callbacks associés.
La méthode on permet d'ajouter un listener (callback) pour un événement spécifique.
La méthode emit déclenche tous les callbacks associés à un événement donné.
Pour l'utiliser dans votre jeu Tamagotchi, vous pourriez l'intégrer comme ceci :
Dans cette version modifiée :
Nous avons ajouté des fonctions configureTerminal() et restoreTerminal() pour configurer le terminal en mode non-canonique (lecture caractère par caractère sans attendre Enter).
Dans la boucle de lecture des entrées, nous vérifions d'abord si le caractère lu est '\033' (échappement), qui indique le début d'une séquence de touche fléchée.
Si c'est une touche fléchée, nous lisons les deux caractères suivants pour déterminer quelle flèche a été pressée.
Nous avons ajouté de nouveaux événements et méthodes pour gérer les mouvements dans les quatre directions.
La configuration et la restauration du terminal sont effectuées au début et à la fin de la méthode run().
Avec ces modifications, votre jeu peut maintenant réagir aux touches fléchées ainsi qu'aux touches normales. Les touches fléchées émettront des événements "up", "down", "left", ou "right" selon la flèche pressée.
N'oubliez pas que cette approche fonctionne principalement sur les systèmes Unix-like (Linux, macOS). Pour Windows, vous devriez utiliser des fonctions spécifiques à Windows ou une bibliothèque multiplateforme comme ncurses pour une gestion plus robuste des entrées clavier.
*/

#include <iostream>
#include <functional>
#include <map>
#include <vector>
#include <string>
#include <thread>
#include <chrono>
#include <termios.h>
#include <unistd.h>
#include <mutex>
#include <condition_variable>

// test variable
int test;
int didYouHearThat;

// forward declaration
class TamagotchiGame;
void display(TamagotchiGame &game, int makeClean);

class EventEmitter {
private:
    std::map<std::string, std::vector<std::function<void()>>> listeners;

public:
    void on(const std::string& event, std::function<void()> callback) {
        listeners[event].push_back(callback);
    }

    void emit(const std::string& event) {
        if (listeners.find(event) != listeners.end()) {
            for (auto& callback : listeners[event]) {
                callback();
            }
        }
    }
};

// Configuration du terminal pour la lecture non bloquante
void configureTerminal() {
    struct termios t;
    tcgetattr(STDIN_FILENO, &t);
    t.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &t);
}

// Restauration de la configuration du terminal
void restoreTerminal() {
    struct termios t;
    tcgetattr(STDIN_FILENO, &t);
    t.c_lflag |= ICANON | ECHO;
    tcsetattr(STDIN_FILENO, TCSANOW, &t);
}

class TamagotchiGame {
private:
    EventEmitter emitter;
    bool running;
    std::mutex mtx;
    std::condition_variable cv;
    char lastInput;
    bool inputAvailable;
    std::vector<std::string> menus;
    size_t activeMenuIndex;

public:
    TamagotchiGame() : running(true), lastInput(0), inputAvailable(false), activeMenuIndex(0) {
        // Configurer les listeners
        emitter.on("quit", [this]() { running = false; });
        emitter.on("left", [this]() { moveLeft(); });
        emitter.on("right", [this]() { moveRight(); });
        emitter.on("select", [this]() { selectMenu(); });

        // Initialiser les menus
        menus = {"feed", "clean", "quit"};
    }

    void moveLeft() {
        if (activeMenuIndex == 0) {
            activeMenuIndex = menus.size() - 1;
        } else {
            activeMenuIndex--;
        }
        didYouHearThat++;
    }

    void moveRight() {
        activeMenuIndex = (activeMenuIndex + 1) % menus.size();
        didYouHearThat++;
    }

    void selectMenu() {
        emitter.emit(menus[activeMenuIndex]);
        didYouHearThat++;
    }

    void displayMenu() {
        for (size_t i = 0; i < menus.size(); ++i) {
            if (i == activeMenuIndex) {
                std::cout << "|" << menus[i] << "| ";
            } else {
                std::cout << " " << menus[i] << "  ";
            }
        }
        std::cout << std::endl;
    }

    void run() {
        configureTerminal();
        std::thread inputThread([this]() {
            while (running) {
                char c = getchar();
                {
                    std::lock_guard<std::mutex> lock(mtx);
                    if (c == '\033') {
                        char next = getchar();
                        if (next == '[') {
                            char direction = getchar();
                            switch(direction) {
                                case 'C': lastInput = 'R'; break; // Droite
                                case 'D': lastInput = 'L'; break; // Gauche
                                default: lastInput = 0; break;
                            }
                        } else {
                            lastInput = 0;
                        }
                    } else {
                        lastInput = c;
                    }
                    inputAvailable = true;
                }
                cv.notify_one();
            }
        });

        auto lastUpdate = std::chrono::steady_clock::now();

        while (running) {
            auto now = std::chrono::steady_clock::now();
            auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastUpdate);

            if (elapsed >= std::chrono::milliseconds(1000)) {
                // Traiter l'entrée si disponible
                {
                    std::unique_lock<std::mutex> lock(mtx);
                    if (inputAvailable) {
                        switch(lastInput) {
                            case 'R': emitter.emit("right"); break;
                            case 'L': emitter.emit("left"); break;
                            case '\n': emitter.emit("select"); break;
                            case 'q': emitter.emit("quit"); break;
                        }
                        inputAvailable = false;
                    }
                }

                // Logique principale du jeu ici
                display(*this, 1);

                lastUpdate = now;
            } else {
                // Attendre soit l'entrée suivante, soit le temps restant jusqu'à la prochaine mise à jour
                std::unique_lock<std::mutex> lock(mtx);
                auto waitTime = std::chrono::milliseconds(1000) - elapsed;
                cv.wait_for(lock, waitTime, [this]{ return inputAvailable; });
            }
        }

        inputThread.join();
        restoreTerminal();
    }
};

void display(TamagotchiGame &game, int makeClean) {
    if (makeClean == 1)
        std::cout << "\033[7A";
    std::cout<<"Did you hear that : "<<didYouHearThat<<" gameloop N°"<<test<<std::endl;
    std::cout<<"()"<<std::endl;
    std::cout<<" . "<<std::endl;
    std::cout<<"---"<<std::endl;
    std::cout<<"/ /"<<std::endl;
    game.displayMenu();
    std::cout<<"end"<<std::endl;

    test++;
    
}

int main() {
    test=0;
    didYouHearThat=0;
    TamagotchiGame game;
    display(game, 0);
    game.run();
    return 0;
}
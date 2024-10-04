#include <iostream>
#include <chrono>
#include <thread>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>

// Fonction pour configurer le mode non bloquant pour la saisie
void setNonBlockingMode() {
    struct termios ttystate;
    tcgetattr(STDIN_FILENO, &ttystate); // Obtient les paramètres actuels du terminal   
    ttystate.c_lflag &= ~ICANON; // Désactive le mode canonique (lecture ligne par ligne)
    ttystate.c_lflag &= ~ECHO;// Désactive l'écho des caractères saisis
    ttystate.c_cc[VMIN] = 1; // Configure pour lire un caractère à la fois
    tcsetattr(STDIN_FILENO, TCSANOW, &ttystate); // Applique les nouveaux paramètres

    // Configure l'entrée standard en mode non bloquant
    int flags = fcntl(STDIN_FILENO, F_GETFL);
    fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);
}

// Fonction pour lire une touche en mode non bloquant
char readKeyNonBlocking() {
    char c;
    // Tente de lire un caractère, retourne 0 si aucun n'est disponible
    if (read(STDIN_FILENO, &c, 1) == 1) {
        return c;
    }
    return 0;
}

int main() {
    // Configure le terminal en mode non bloquant
    setNonBlockingMode();

    int test = 0;
    int compteur = 0;
    char touche;

    while (true) {
        // Marque le début de la boucle d'une seconde
        auto debut = std::chrono::steady_clock::now();
        char lastTouche = 0;

        do {
            touche = readKeyNonBlocking();
            if (touche != 0) {
                // Traitement de la touche pressée
                if (touche == 'q') {
                    std::cout << "Sortie du programme." << std::endl;
                    return 0;  // Quitte le programme si 'q' est pressé
                }
                lastTouche = touche;  // Enregistre la dernière touche pressée
            }
            // Pause courte pour éviter une utilisation excessive du CPU
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        } while (std::chrono::duration_cast<std::chrono::seconds>(
                     std::chrono::steady_clock::now() - debut)
                     .count() < 1);  // Continue la boucle pendant 1 seconde

        test++;
        if (lastTouche == '+') {
            compteur++;
        } else if (lastTouche == '-') {
            compteur--;
        }

        std::cout << test << " Compteur : " << compteur << " last touche = "<< lastTouche << std::endl;
    }

    return 0;
}
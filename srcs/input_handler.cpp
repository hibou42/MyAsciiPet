#include "../include/input_handler.hpp"
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>

void configureTerminal() {
    struct termios ttystate;  // Declare a structure to store terminal parameters
    tcgetattr(STDIN_FILENO, &ttystate);  // Get current terminal parameters

    ttystate.c_lflag &= ~ICANON;  // Disable canonical mode (line-by-line input)
    ttystate.c_lflag &= ~ECHO;    // Disable echoing of characters
    ttystate.c_cc[VMIN] = 1;      // Configure to return after reading at least 1 character

    tcsetattr(STDIN_FILENO, TCSANOW, &ttystate);  // Apply new parameters to the terminal

    int flags = fcntl(STDIN_FILENO, F_GETFL);  // Get current flags of the file descriptor
    fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);  // Enable non-blocking mode for reading
}

void resetTerminal() {
    struct termios ttystate;  // Declare a structure to store terminal parameters
    tcgetattr(STDIN_FILENO, &ttystate);  // Get current terminal parameters

    ttystate.c_lflag |= ICANON | ECHO;  // Re-enable canonical mode and echo

    tcsetattr(STDIN_FILENO, TCSANOW, &ttystate);  // Apply new parameters to the terminal

    int flags = fcntl(STDIN_FILENO, F_GETFL);  // Get current flags of the file descriptor
    fcntl(STDIN_FILENO, F_SETFL, flags & ~O_NONBLOCK);  // Disable non-blocking mode
}

char readKeyNonBlocking() {
    char c;
    if (read(STDIN_FILENO, &c, 1) == 1) {
        return c;
    }
    return 0;
}

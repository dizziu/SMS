#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <termios.h>
#include <unistd.h>
#include "utils.h"

// Clears the Screen
void clear_screen() {
    system("clear");
}

// Waits for users to use enter
void wait_enter() {
    printf("\nPress ENTER to continue...");
    getchar();
    getchar();
}

// Shows current time(linux only)
void show_time() {
    time_t now = time(NULL);
    printf("%s\n", ctime(&now));
}

// Hide input(mainly used for password) by changing terminal behaviour
void hidden_input(char *buffer) {
    struct termios old, new; // Declare terminal settings

    tcgetattr(STDIN_FILENO, &old); // Get current terminal settings
    new = old; // Copy settings
    new.c_lflag &= ~ECHO;  // Disable echo
    tcsetattr(STDIN_FILENO, TCSANOW, &new); // Apply new settings

    scanf("%s", buffer); // Read input

    tcsetattr(STDIN_FILENO, TCSANOW, &old); // Restore original settings
}

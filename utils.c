#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#ifdef _WIN32
#include <conio.h>
#include <windows.h>
#else
#include <termios.h>
#include <unistd.h>
#endif

#include "utils.h"

// Clears the Screen
void clear_screen() {
#ifdef _WIN32
  system("cls");
#else
  system("clear");
#endif
}

// Waits for users to use enter
void wait_enter() {
  printf("\nPress ENTER to continue...");
#ifdef _WIN32
  getch();
#else
  getchar();
  getchar();
#endif
}

void show_time() {
  time_t now = time(NULL);
  printf("%s\n", ctime(&now));
}

// Hide input(mainly used for password) by changing terminal behaviour
void hidden_input(char *buffer) {

#ifdef _WIN32
  int i = 0;
  char ch;

  while ((ch = _getch()) != '\r') {
    buffer[i++] = ch;
    printf("*");
  }
  buffer[i] = '\0';

#else
  struct termios old, new; // Declare terminal settings

  tcgetattr(STDIN_FILENO, &old);          // Get current terminal settings
  new = old;                              // Copy settings
  new.c_lflag &= ~ECHO;                   // Disable echo
  tcsetattr(STDIN_FILENO, TCSANOW, &new); // Apply new settings

  scanf("%s", buffer); // Read input

  tcsetattr(STDIN_FILENO, TCSANOW, &old); // Restore original settings
#endif
}

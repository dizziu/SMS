#include "auth.h"
#include "utils.h"
#include <stdio.h>
#include <string.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

static char stored[100];

static void slp() {
#ifdef _WIN32
  Sleep(1000); // 1 second = 1000 ms
#else
  sleep(1); // 1 second
#endif
}
void login_system() {
  int i = 0;
  FILE *fp = fopen("password.txt", "r");

  if (fp) {
    fscanf(fp, "%s", stored);
    fclose(fp);
  } else {
    strcpy(stored, "admin");
    i = 1;
  }

  char user[50], pass[100];

  while (1) {
    clear_screen();
    show_time();
    if (i) {
      printf("Username: admin \n");
      printf("Default Password: admin \n");
    }

    printf("\nUsername: ");
    scanf("%s", user);

    printf("Password: ");
    hidden_input(pass);

    if (strcmp(user, "admin") == 0 && strcmp(pass, stored) == 0) {
      printf("\nLogin Successful!\n");
      // user get some time to see the text
      slp();
      break;
    }

    printf("\nInvalid Credentials!\n");
    slp();
  }
}

void change_password() {
  char newp[100], conf[100];

  clear_screen();
  printf("\n--- Change Password ---\n");

  printf("New Password: ");
  hidden_input(newp);

  printf("\nConfirm Password: ");
  hidden_input(conf);

  if (strcmp(newp, conf) != 0) {
    printf("\nPasswords do not match!\n");
  } else {
    FILE *fp = fopen("password.txt", "w");
    fprintf(fp, "%s", newp);
    fclose(fp);
    printf("\nPassword Updated!\n");
  }

  wait_enter();
}

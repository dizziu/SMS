#include "auth.h"
#include "student.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>

int main() {

  init_data_file();
  login_system();

  int choice;

  while (1) {
    clear_screen();

    printf("\n==== STUDENT MANAGEMENT SYSTEM ====\n");
    printf("1. Add Student\n");
    printf("2. Display Students\n");
    printf("3. Delete Student\n");
    printf("4. Search Student\n");
    printf("5. Update Student\n");
    printf("6. Show Statistics\n");
    printf("7. Change Password\n");
    printf("8. Exit\n");

    printf("Choice: ");
    scanf("%d", &choice);

    switch (choice) {
    case 1:
      add_student();
      break;
    case 2:
      display_students();
      break;
    case 3:
      delete_student();
      break;
    case 4:
      search_student();
      break;
    case 5:
      update_student();
      break;
    case 6:
      show_statistics();
      break;
    case 7:
      change_password();
      break;
    case 8:
      exit(0);
    default:
      printf("Invalid Option!\n");
    }
  }

  return 0;
}

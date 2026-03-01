#include "student.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Use of static here means that the variable is only defined within this file and doesn't link to other file
static FILE *dataFile;
static Student temp, *head = NULL;

// initialization of the main file where all our data is stored
void init_data_file() {
  dataFile = fopen("students.dat", "rb+");
  if (!dataFile)
    dataFile = fopen("students.dat", "wb+");
}

// initialization of the linked list
static void load_list() {
  rewind(dataFile);
  head = NULL;

  Student *node, *last = NULL;

  // here the fread reads exatcly the size of "Student" struct from the file at one time and return 1 if it's successful and 0 if end of file(uses binary mode)
  while (fread(&temp, sizeof(Student), 1, dataFile) == 1) {
    node = malloc(sizeof(Student));
    *node = temp;
    node->next = NULL;

    if (!head)
      head = node;
    else
      last->next = node;

    last = node;
  }
}


// Storing the data given by user to the file
void add_student() {
  clear_screen();
  printf("\n--- Add Student ---\n");

  printf("First Name: ");
  scanf("%s", temp.fname);

  printf("Last Name: ");
  scanf("%s", temp.lname);

  printf("ID: ");
  scanf("%s", temp.id);

  printf("Gender: ");
  scanf("%s", temp.gender);

  printf("Father: ");
  scanf("%s", temp.father);

  printf("Mother: ");
  scanf("%s", temp.mother);

  printf("DOB: ");
  scanf("%s", temp.dob);

  fwrite(&temp, sizeof(Student), 1, dataFile);

  printf("\nStudent Added!\n");
  wait_enter();
}

// Displaying record using the linked list
void display_students() {
  clear_screen();
  load_list();

  Student *curr = head;

  printf("\n--- Student List ---\n");

  while (curr) {
    printf("%s %s | ID: %s\n", curr->fname, curr->lname, curr->id);
    curr = curr->next;
  }

  wait_enter();
}

// Search for the student in the linked list
void search_student() {
  clear_screen();
  load_list();

  char sid[30];
  printf("Enter ID: ");
  scanf("%s", sid);

  Student *curr = head;
  int found = 0;

  while (curr) {
    if (strcmp(curr->id, sid) == 0) {
      printf("\nName: %s %s", curr->fname, curr->lname);
      printf("\nFather: %s", curr->father);
      printf("\nMother: %s", curr->mother);
      printf("\nGender: %s", curr->gender);
      printf("\nDOB: %s\n", curr->dob);
      found = 1;
      break;
    }
    curr = curr->next;
  }

  if (!found)
    printf("\nNot Found!\n");

  wait_enter();
}

// Update the student data
void update_student() {
  clear_screen();

  FILE *tmp = fopen("temp.dat", "wb");
  char sid[30];
  int updated = 0;

  printf("Enter ID to update: ");
  scanf("%s", sid);

  rewind(dataFile);

  while (fread(&temp, sizeof(Student), 1, dataFile) == 1) {

    if (strcmp(temp.id, sid) == 0) {
      updated = 1;

      printf("\nEnter new details:\n");

      printf("First Name: ");
      scanf("%s", temp.fname);

      printf("Last Name: ");
      scanf("%s", temp.lname);

      printf("Gender: ");
      scanf("%s", temp.gender);

      printf("Father: ");
      scanf("%s", temp.father);

      printf("Mother: ");
      scanf("%s", temp.mother);

      printf("DOB: ");
      scanf("%s", temp.dob);
    }

    fwrite(&temp, sizeof(Student), 1, tmp);
  }

  fclose(dataFile);
  fclose(tmp);

  remove("students.dat");
  rename("temp.dat", "students.dat");

  init_data_file();

  if (updated)
    printf("\nRecord updated successfully!\n");
  else
    printf("\nStudent not found!\n");

  wait_enter();
}

// Delete the data
void delete_student() {
  clear_screen();

  FILE *tmp = fopen("temp.dat", "wb");
  char sid[30];
  int removed = 0;

  printf("Enter ID to delete: ");
  scanf("%s", sid);

  rewind(dataFile);

  while (fread(&temp, sizeof(Student), 1, dataFile) == 1) {
    if (strcmp(temp.id, sid) != 0)
      fwrite(&temp, sizeof(Student), 1, tmp);
    else
      removed = 1;
  }

  fclose(dataFile);
  fclose(tmp);

  remove("students.dat");
  rename("temp.dat", "students.dat");

  init_data_file();

  if (removed)
    printf("\nDeleted Successfully!\n");
  else
    printf("\nStudent Not Found!\n");

  wait_enter();
}

// Show no. of male and female in the records
void show_statistics() {
    clear_screen();

    int male = 0, female = 0, total = 0;

    rewind(dataFile);

    while (fread(&temp, sizeof(Student), 1, dataFile) == 1) {
        total++;

        if (temp.gender[0] == 'M' || temp.gender[0] == 'm')
            male++;
        else if (temp.gender[0] == 'F' || temp.gender[0] == 'f')
            female++;
    }

    printf("\n--- Student Statistics ---\n");
    printf("Total Students : %d\n", total);
    printf("Male Students  : %d\n", male);
    printf("Female Students: %d\n", female);

    wait_enter();
}

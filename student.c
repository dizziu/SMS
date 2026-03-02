#include "student.h"
#include "auth.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* =========================
   Hash Function
   ========================= */

unsigned int hash(const char *id) {
  unsigned int h = 0;
  while (*id) {
    h = (h * 31) + *id++;
  }
  return h % TABLE_SIZE;
}

/* =========================
   Database Core
   ========================= */

void db_init(Database *db) {
  for (int i = 0; i < TABLE_SIZE; i++)
    db->buckets[i] = NULL;
  db->count = 0;
}

void db_add(Database *db, Student s) {
  unsigned int index = hash(s.id);

  Node *newNode = malloc(sizeof(Node));
  newNode->data = s;
  newNode->next = db->buckets[index];
  db->buckets[index] = newNode;

  db->count++;
}

Student *db_find(Database *db, const char *id) {
  clear_screen();
  unsigned int index = hash(id);
  Node *current = db->buckets[index];

  while (current) {
    if (strcmp(current->data.id, id) == 0)
      return &current->data;
    current = current->next;
  }
  return NULL;
  wait_enter();
}

void db_delete(Database *db, const char *id) {
  unsigned int index = hash(id);
  Node *current = db->buckets[index];
  Node *prev = NULL;

  while (current) {
    if (strcmp(current->data.id, id) == 0) {
      if (prev)
        prev->next = current->next;
      else
        db->buckets[index] = current->next;

      free(current);
      db->count--;
      printf("Student deleted.\n");
      return;
    }
    prev = current;
    current = current->next;
  }
  printf("Student not found.\n");
  wait_enter();
}

void db_free(Database *db) {
  for (int i = 0; i < TABLE_SIZE; i++) {
    Node *current = db->buckets[i];
    while (current) {
      Node *temp = current;
      current = current->next;
      free(temp);
    }
  }
}

/* =========================
   Persistence (Snapshot)
   ========================= */

void save_snapshot(Database *db) {
  FILE *file = fopen(DATA_FILE, "wb");
  if (!file) {
    printf("Error saving file.\n");
    return;
  }

  for (int i = 0; i < TABLE_SIZE; i++) {
    Node *current = db->buckets[i];
    while (current) {
      fwrite(&current->data, sizeof(Student), 1, file);
      current = current->next;
    }
  }

  fclose(file);
  printf("Data saved successfully.\n");
}

void load_snapshot(Database *db) {
  FILE *file = fopen(DATA_FILE, "rb");
  if (!file)
    return;

  Student s;
  while (fread(&s, sizeof(Student), 1, file) == 1) {
    db_add(db, s);
  }

  fclose(file);
}

/* =========================
   User Interface
   ========================= */

void input_student(Student *s) {
  clear_screen();
  printf("ID: ");
  scanf("%19s", s->id);

  printf("First Name: ");
  scanf("%49s", s->fname);

  printf("Last Name: ");
  scanf("%49s", s->lname);

  printf("Gender: ");
  scanf("%99s", s->gender);
  printf("Father: ");
  scanf("%99s", s->father);
  printf("Mother: ");
  scanf("%99s", s->mother);
  printf("DOB: ");
  scanf("%19s", s->dob);
}

void db_update(Database *db, Student s, char *id) {
  int updated = 0;
  if (db_find(db, id) != NULL) {
    updated = 1;
    strcpy(s.id, id); // keep same ID
    printf("Enter new details:\n");
    printf("First Name: ");
    scanf("%49s", s.fname);

    printf("Last Name: ");
    scanf("%49s", s.lname);

    printf("Gender: ");
    scanf("%99s", s.gender);

    printf("Father: ");
    scanf("%99s", s.father);

    printf("Mother: ");
    scanf("%99s", s.mother);

    printf("DOB: ");
    scanf("%19s", s.dob);

    unsigned int index = hash(s.id);
    Node *current = db->buckets[index];
    while (current) {
      if (strcmp(current->data.id, s.id) == 0) {
        current->data = s; // overwrite entire struct
        printf("\nRecord updated successfully!\n");
        return;
      }
      current = current->next;
    }
  }
  if (!updated)
    printf("Record not found! \n");

  wait_enter();
}

void print_student(Student *s) {
  printf("\nID: %s\n", s->id);
  printf("First Name: %s\n", s->fname);
  printf("Last Name: %s\n", s->lname);
  printf("Gender: %s\n", s->gender);
  printf("Father: %s\n", s->father);
  printf("Mother: %s\n", s->mother);
  printf("DOB: %s\n", s->dob);
}

void db_display_all(Database *db) {
  clear_screen();
  if (db->count == 0) {
    printf("No students in database.\n");
    wait_enter();
    return;
  }

  printf("\n=== All Students (%u total) ===\n", db->count);

  for (int i = 0; i < TABLE_SIZE; i++) {
    Node *current = db->buckets[i];

    while (current) {
      print_student(&current->data);
      printf("----------------------\n");
      current = current->next;
    }
  }
  wait_enter();
}

void db_show_stats(Database *db) {
  clear_screen();
  int used_buckets = 0;
  int max_chain = 0;
  int total_chain_nodes = 0;

  for (int i = 0; i < TABLE_SIZE; i++) {
    Node *current = db->buckets[i];
    int chain_length = 0;

    if (current)
      used_buckets++;

    while (current) {
      chain_length++;
      current = current->next;
    }

    if (chain_length > max_chain)
      max_chain = chain_length;

    total_chain_nodes += chain_length;
  }

  double load_factor = (double)db->count / TABLE_SIZE;
  double avg_chain =
      used_buckets ? (double)total_chain_nodes / used_buckets : 0;

  printf("\n==== Database Stats ====\n");
  printf("Total Students     : %u\n", db->count);
  printf("Table Size         : %d\n", TABLE_SIZE);
  printf("Used Buckets       : %u\n", used_buckets);
  printf("Empty Buckets      : %d\n", TABLE_SIZE - (int)used_buckets);
  printf("Load Factor        : %.3f\n", load_factor);
  printf("Max Chain Length   : %u\n", max_chain);
  printf("Avg Chain Length   : %.3f\n", avg_chain);

  wait_enter();
}

void menu(Database *db) {

  int choice;
  char id[20];
  Student s;

  login_system();
  while (1) {
    clear_screen();
    printf("\n==== Student DB ====\n");
    printf("1. Add Student\n");
    printf("2. Find Student\n");
    printf("3. Delete Student\n");
    printf("4. Update Student\n");
    printf("5. Display All Students\n");
    printf("6. Change password\n");
    printf("7. Show Stats\n");
    printf("8. Exit\n");
    printf("Choice:");
    scanf("%d", &choice);

    switch (choice) {
    case 1:
      input_student(&s);
      db_add(db, s);
      printf("Student added.\n");
      break;

    case 2:
        clear_screen();
      printf("Enter ID: ");
      scanf("%19s", id);
      Student *found = db_find(db, id);
      if (found)
        print_student(found);
      else
        printf("Student not found.\n");
      break;

    case 3:
      clear_screen();
      printf("Enter ID: ");
      scanf("%19s", id);
      db_delete(db, id);
      break;

    case 4:
      printf("Enter ID to update: ");
      scanf("%19s", id);

      db_update(db, s, id);
      break;

    case 5:
      db_display_all(db);
      break;

    case 6:
      change_password();
      break;

    case 7:
      db_show_stats(db);
      break;

    case 8:
      return;

    default:
      printf("Invalid choice.\n");
      return;
    }
  }
}

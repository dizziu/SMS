// This is an header guard
#ifndef STUDENT_H
#define STUDENT_H

#define TABLE_SIZE 67
#define DATA_FILE "students.dat"

// Student definition as a linked list
typedef struct {
  char id[20];
  char fname[50];
  char lname[50];
  char gender[10];
  char father[100];
  char mother[100];
  char dob[30];
} Student;

typedef struct Node {
  Student data;
  struct Node *next;
} Node;

typedef struct {
  Node *buckets[TABLE_SIZE];
  int count;
} Database;

unsigned int hash(const char *id); 
void db_init(Database *db); 
void db_add(Database *db, Student s); 
void db_free(Database *db);
void save_snapshot(Database *db);
void load_snapshot(Database *db);


void input_student(Student *s);
void db_update(Database *db, Student s, char *id); 
void print_student(Student *s);
void db_show_stats(Database *db);
void menu(Database *db); 

#endif

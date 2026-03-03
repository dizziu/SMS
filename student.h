// This is an header guard
#ifndef STUDENT_H
#define STUDENT_H

// Student definition as a linked list
typedef struct Student {
    char fname[50];
    char lname[50];
    char id[30];
    char gender[10];
    char father[50];
    char mother[50];
    char dob[30];
    struct Student *next;
} Student;

void init_data_file();
void add_student();
void display_students();
void search_student();
void delete_student();
void show_statistics();
void update_student();

#endif

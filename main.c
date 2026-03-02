#include "student.h"


int main() {
  Database db;

  db_init(&db);
  load_snapshot(&db);

  menu(&db);

  save_snapshot(&db);
  db_free(&db);

  return 0;
}

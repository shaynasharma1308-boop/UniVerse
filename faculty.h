#ifndef FACULTY_H
#define FACULTY_H

#include <string>

void loadFacultyData();
void studentFacultyMenu();
void adminFacultyMenu();

bool facultyExists(int id);
std::string getFacultyNameByID(int id);

#endif
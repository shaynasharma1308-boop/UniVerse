#ifndef CLUBS_H
#define CLUBS_H

#include <string>
#include <vector>
#include <utility>

void loadClubData();
void studentClubMenu();
void adminClubMenu();

bool clubExists(int id);
std::string getClubNameByID(int clubID);
std::vector<std::pair<int, int>> getClubFacultyLinks();
std::string getClubNamesForFaculty(int facultyID);
void removeFacultyCoordinator(int facultyID);

#endif
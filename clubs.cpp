#include <iostream>
#include <vector>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include <string>
#include "clubs.h"
#include "faculty.h"
using namespace std;

struct Club {
    int id;
    string name;
    string description;
    int facultyCoordinatorID;
    string president;
    string vicePresident;
    vector<string> councilMembers;
};
vector<Club> clubs;
unordered_map<int, int> clubIndex;

const string FILE_NAME = "clubs.txt";
const string ADMIN_PASSWORD = "admin123";

void rebuildClubIndex() {
    clubIndex.clear();
    for (int i = 0; i < clubs.size(); i++) {
        clubIndex[clubs[i].id] = i;
    }
}

void saveClubData() {
    ofstream file(FILE_NAME);
    for (const Club &c : clubs) {
        file << c.id << '|'
             << c.name << '|'
             << c.description << '|'
             << c.facultyCoordinatorID << '|'
             << c.president << '|'
             << c.vicePresident << '|';

        for (int i = 0; i < c.councilMembers.size(); i++) {
            if (i > 0) file << ';';
            file << c.councilMembers[i];
        }
        file << '\n';
    }
    file.close();
}
void loadClubData() {
    ifstream file(FILE_NAME);
    string line;
    clubs.clear();
    clubIndex.clear();

    while (getline(file, line)) {
        stringstream ss(line);
        string part;
        vector<string> fields;

        while (getline(ss, part, '|')) fields.push_back(part);
        if (!line.empty() && line.back() == '|') fields.push_back("");
        if (fields.size() != 7) continue;

        try {
            Club c;
            c.id = stoi(fields[0]);
            if (clubIndex.count(c.id)) continue;

            c.name = fields[1];
            c.description = fields[2];
            // Old records may contain a coordinator name instead of an ID.
            // Keep the club record and leave its coordinator unassigned.
            try {
                c.facultyCoordinatorID = stoi(fields[3]);
            } catch (...) {
                c.facultyCoordinatorID = 0;
            }
            c.president = fields[4];
            c.vicePresident = fields[5];

            stringstream members(fields[6]);
            string member;
            while (getline(members, member, ';')) {
                if (!member.empty()) c.councilMembers.push_back(member);
            }

            clubs.push_back(c);
            clubIndex[c.id] = clubs.size() - 1;
        } catch (...) {
            continue;
        }
    }
    file.close();
}

int searchByClubID(int id) {
    if (clubIndex.count(id)) return clubIndex[id];
    return -1;
}

bool clubExists(int id) {
    return searchByClubID(id) != -1;
}
string getClubNamesForFaculty(int facultyID) {
    string result;
    for (const Club &c : clubs) {
        if (c.facultyCoordinatorID == facultyID) {
            if (!result.empty()) result += ", ";
            result += c.name;
        }
    }
    return result;
}


vector<pair<int, int>> getClubFacultyLinks() {
    vector<pair<int, int>> links;
    for (const Club &c : clubs) {
        if (c.facultyCoordinatorID != 0) {
            links.push_back({c.facultyCoordinatorID, c.id});
        }
    }
    return links;
}

string getClubNameByID(int clubID) {
    int index = searchByClubID(clubID);
    if (index == -1) return "";
    return clubs[index].name;
}
void removeFacultyCoordinator(int facultyID) {
    bool changed = false;
    for (Club &c : clubs) {
        if (c.facultyCoordinatorID == facultyID) {
            c.facultyCoordinatorID = 0;
            changed = true;
        }
    }
    if (changed) saveClubData();
}

void displayClub(const Club &c) {
    cout << "\n========== CLUB DETAILS ==========\n";
    cout << "Club ID: " << c.id << '\n';
    cout << "Club Name: " << c.name << '\n';
    cout << "Description: " << c.description << '\n';

    if (c.facultyCoordinatorID == 0) {
        cout << "Faculty Coordinator: Not assigned\n";
    } else {
        string facultyName = getFacultyNameByID(c.facultyCoordinatorID);
        if (facultyName.empty()) cout << "Faculty Coordinator: Faculty record not found\n";
        else cout << "Faculty Coordinator: " << facultyName << " (ID: " << c.facultyCoordinatorID << ")\n";
    }

    cout << "President: " << c.president << '\n';
    cout << "Vice President: " << c.vicePresident << '\n';
    cout << "Council Members: ";
if (c.councilMembers.empty()) {
        cout << "Not available";
    } else {
        for (int i = 0; i < c.councilMembers.size(); i++) {
            cout << c.councilMembers[i];
            if (i < c.councilMembers.size() - 1) cout << ", ";
        }
    }
    cout << "\n==================================\n";
}

void displayAllClubs() {
    if (clubs.empty()) {
        cout << "\nNo clubs available.\n";
        return;
    }
    for (const Club &c : clubs) displayClub(c);
}

void searchClub() {
    int id;
    cout << "Enter Club ID: ";
    if (!(cin >> id)) {
        cin.clear();
        cin.ignore(10000, '\n');
        cout << "Invalid ID.\n";
        return;
    }
    int index = searchByClubID(id);
    if (index == -1) cout << "Club not found.\n";
    else displayClub(clubs[index]);
}

void searchByClubName() {
    string name;
    bool found = false;
    cout << "Enter Club Name: ";
    getline(cin >> ws, name);

    for (const Club &c : clubs) {
        if (c.name.find(name) != string::npos) {
            displayClub(c);
            found = true;
        }
    }
    if (!found) cout << "No matching club found.\n";
}

bool readCoordinatorID(int &facultyID) {
    cout << "Enter Faculty Coordinator ID (0 if not assigned): ";
    if (!(cin >> facultyID)) {
        cin.clear();
        cin.ignore(10000, '\n');
        cout << "Invalid ID.\n";
        return false;
    }
     if (facultyID != 0 && !facultyExists(facultyID)) {
        cout << "Faculty ID does not exist. Add the faculty member first.\n";
        return false;
    }
    return true;
}

void addClub() {
    Club c;
    cout << "Enter Club ID: ";
    if (!(cin >> c.id)) {
        cin.clear();
        cin.ignore(10000, '\n');
        cout << "Invalid ID.\n";
        return;
    }
    if (searchByClubID(c.id) != -1) {
        cout << "Club ID already exists.\n";
        return;
    }
    cout << "Enter Club Name: ";
    getline(cin >> ws, c.name);
    cout << "Enter Description: ";
    getline(cin, c.description);
    if (!readCoordinatorID(c.facultyCoordinatorID)) return;
    cout << "Enter Student President: ";
    getline(cin >> ws, c.president);
    cout << "Enter Student Vice President: ";
    getline(cin, c.vicePresident);
    int n;
    cout << "Enter Number of Other Council Members: ";
    if (!(cin >> n) || n < 0 || n > 100) {
        cin.clear();
        cin.ignore(10000, '\n');
        cout << "Invalid number of members.\n";
        return;
    }
    for (int i = 0; i < n; i++) {
        string member;
        cout << "Enter Council Member " << i + 1 << ": ";
        getline(cin >> ws, member);
        c.councilMembers.push_back(member);
    }

    clubs.push_back(c);
    rebuildClubIndex();
    saveClubData();
    cout << "Club added successfully!\n";
}

void updateClub() {
    int id;
    cout << "Enter Club ID to Update: ";
    if (!(cin >> id)) {
        cin.clear();
        cin.ignore(10000, '\n');
        cout << "Invalid ID.\n";
        return;
    }
     int index = searchByClubID(id);
    if (index == -1) {
        cout << "Club not found.\n";
        return;
    }

    Club updated = clubs[index];
    cout << "Enter New Club Name: ";
    getline(cin >> ws, updated.name);
    cout << "Enter New Description: ";
    getline(cin, updated.description);
    if (!readCoordinatorID(updated.facultyCoordinatorID)) return;
    cout << "Enter New President: ";
    getline(cin >> ws, updated.president);
    cout << "Enter New Vice President: ";
    getline(cin, updated.vicePresident);

    int n;
    cout << "Enter Number of Other Council Members: ";
    if (!(cin >> n) || n < 0 || n > 100) {
        cin.clear();
        cin.ignore(10000, '\n');
        cout << "Invalid number of members. Update cancelled.\n";
        return;
    }
     vector<string> newMembers;
    for (int i = 0; i < n; i++) {
        string member;
        cout << "Enter Council Member " << i + 1 << ": ";
        getline(cin >> ws, member);
        newMembers.push_back(member);
    }
    updated.councilMembers = newMembers;
    clubs[index] = updated;
    saveClubData();
    cout << "Club updated successfully!\n";
}
void deleteClub() {
    int id;
    cout << "Enter Club ID to Delete: ";
    if (!(cin >> id)) {
        cin.clear();
        cin.ignore(10000, '\n');
        cout << "Invalid ID.\n";
        return;
    }
    int index = searchByClubID(id);
    if (index == -1) {
        cout << "Club not found.\n";
        return;
    }
    clubs.erase(clubs.begin() + index);
    rebuildClubIndex();
    saveClubData();
    cout << "Club deleted successfully!\n";
}

bool adminLogin() {
    string password;
    cout << "Enter Admin Password: ";
    cin >> password;
    if (password == ADMIN_PASSWORD) {
        cout << "Login successful!\n";
        return true;
    }
    cout << "Incorrect password!\n";
    return false;
}

void studentClubMenu() {
    int choice;
    do {
        cout << "\n========== CLUBS DIRECTORY ==========" << '\n';
        cout << "1. View All Clubs\n";
        cout << "2. Search Club by ID\n";
        cout << "3. Search Club by Name\n";
        cout << "4. Back to Main Menu\n";
        cout << "Enter Choice: ";
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Invalid input.\n";
            continue;
        }
        switch (choice) {
            case 1: displayAllClubs(); break;
            case 2: searchClub(); break;
            case 3: searchByClubName(); break;
            case 4: break;
            default: cout << "Invalid choice.\n";
        }
    } while (choice != 4);
}

void adminClubMenu() {
    if (!adminLogin()) return;
    int choice;
    do {
        cout << "\n========== CLUB ADMIN MENU ==========" << '\n';
        cout << "1. Add Club\n";
        cout << "2. View All Clubs\n";
        cout << "3. Search Club by ID\n";
        cout << "4. Search Club by Name\n";
        cout << "5. Update Club\n";
        cout << "6. Delete Club\n";
        cout << "7. Logout\n";
        cout << "Enter Choice: ";
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Invalid input.\n";
            continue;
        }
         switch (choice) {
            case 1: addClub(); break;
            case 2: displayAllClubs(); break;
            case 3: searchClub(); break;
            case 4: searchByClubName(); break;
            case 5: updateClub(); break;
            case 6: deleteClub(); break;
            case 7: cout << "Logged out successfully.\n"; break;
            default: cout << "Invalid choice.\n";
        }
    } while (choice != 7);
}





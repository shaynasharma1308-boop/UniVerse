#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>
#include <set>
#include "graph.h"
#include "clubs.h"
#include "faculty.h"

using namespace std;

unordered_map<string, vector<string>> adjacencyList;

string facultyNode(int id) {
    return "F" + to_string(id);
}

string clubNode(int id) {
    return "C" + to_string(id);
}

void buildGraph() {
    adjacencyList.clear();

    vector<pair<int, int>> links = getClubFacultyLinks();

    for (const auto &link : links) {
        int facultyID = link.first;
        int clubID = link.second;

        if (!facultyExists(facultyID)) continue;

        string faculty = facultyNode(facultyID);
        string club = clubNode(clubID);

        adjacencyList[faculty].push_back(club);
        adjacencyList[club].push_back(faculty);
    }
}

void displayGraph() {
    buildGraph();

    cout << "\n========== CLUB-FACULTY GRAPH ==========\n";

    vector<pair<int, int>> links = getClubFacultyLinks();
    set<int> facultyIDs;

    for (const auto &link : links) {
        if (facultyExists(link.first)) {
            facultyIDs.insert(link.first);
        }
    }

    if (facultyIDs.empty()) {
        cout << "No faculty-club connections are available yet.\n";
    } else {
        cout << "\nAdjacency List:\n";

        for (int facultyID : facultyIDs) {
            cout << "Faculty " << facultyID << " ("
                 << getFacultyNameByID(facultyID) << ") -> ";

            vector<string> connectedClubs =
                adjacencyList[facultyNode(facultyID)];

            for (int i = 0; i < connectedClubs.size(); i++) {
                int clubID = stoi(connectedClubs[i].substr(1));

                cout << "Club " << clubID << " ("
                     << getClubNameByID(clubID) << ")";

                if (i < connectedClubs.size() - 1) cout << ", ";
            }

            cout << '\n';
        }

        cout << "\nReverse Connections:\n";

        for (const auto &link : links) {
            int facultyID = link.first;
            int clubID = link.second;

            if (!facultyExists(facultyID)) continue;

            cout << "Club " << clubID << " ("
                 << getClubNameByID(clubID)
                 << ") -> Faculty " << facultyID << " ("
                 << getFacultyNameByID(facultyID) << ")\n";
        }
    }

    cout << "========================================\n";
}

void searchFacultyConnections() {
    int id;

    cout << "\nEnter Faculty ID: ";

    if (!(cin >> id)) {
        cin.clear();
        cin.ignore(10000, '\n');
        cout << "Invalid ID.\n";
        return;
    }

    if (!facultyExists(id)) {
        cout << "Faculty not found.\n";
        return;
    }

    buildGraph();

    string node = facultyNode(id);

    cout << "\nFaculty: " << getFacultyNameByID(id)
         << " (ID: " << id << ")\n";

    if (adjacencyList[node].empty()) {
        cout << "No clubs are assigned to this faculty member.\n";
        return;
    }

    cout << "Connected Clubs:\n";

    for (const string &club : adjacencyList[node]) {
        int clubID = stoi(club.substr(1));

        cout << "- " << getClubNameByID(clubID)
             << " (ID: " << clubID << ")\n";
    }
}

void searchClubCoordinator() {
    int id;

    cout << "\nEnter Club ID: ";

    if (!(cin >> id)) {
        cin.clear();
        cin.ignore(10000, '\n');
        cout << "Invalid ID.\n";
        return;
    }

    if (!clubExists(id)) {
        cout << "Club not found.\n";
        return;
    }

    buildGraph();

    string node = clubNode(id);

    cout << "\nClub: " << getClubNameByID(id)
         << " (ID: " << id << ")\n";

    if (adjacencyList[node].empty()) {
        cout << "No faculty coordinator is assigned to this club.\n";
        return;
    }

    for (const string &faculty : adjacencyList[node]) {
        int facultyID = stoi(faculty.substr(1));

        cout << "Faculty Coordinator: "
             << getFacultyNameByID(facultyID)
             << " (ID: " << facultyID << ")\n";
    }
}

void graphMenu() {
    int choice;

    do {
        cout << "\n========== CLUB-FACULTY GRAPH ==========\n";
        cout << "1. Display All Connections\n";
        cout << "2. Search Clubs by Faculty ID\n";
        cout << "3. Search Faculty by Club ID\n";
        cout << "4. Back\n";
        cout << "Enter Choice: ";

        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Invalid input.\n";
            continue;
        }

        switch (choice) {
            case 1: displayGraph(); break;
            case 2: searchFacultyConnections(); break;
            case 3: searchClubCoordinator(); break;
            case 4: break;
            default: cout << "Invalid choice.\n";
        }

    } while (choice != 4);
}
#include <iostream>
#include "clubs.h"
#include "faculty.h"
#include "graph.h"
using namespace std;

int main() {

    loadFacultyData();
    loadClubData();

    int choice;

    do {
        cout << "\n========== UNIVERSE ==========\n";
        cout << "Your University, One Platform\n\n";

        cout << "1. Student Portal\n";
        cout << "2. Admin Portal\n";
        cout << "3. Exit\n";

        cout << "\nEnter your choice: ";
        cin >> choice;

        switch (choice) {

            case 1: {
                int option;
                 do {
                    cout << "\n========== STUDENT PORTAL ==========\n";
                    cout << "1. Clubs Directory\n";
                    cout << "2. Faculty Directory\n";
                    cout << "3. Club-Faculty Graph\n";
                    cout << "4. Back\n";

                    cout << "\nEnter your choice: ";
                    cin >> option;

                    switch (option) {
                        case 1:
                            studentClubMenu();
                            break;

                        case 2:
                            studentFacultyMenu();
                            break;

                        case 3:
                            graphMenu();
                            break;

                        case 4:
                            break;

                        default:
                            cout << "\nInvalid choice.\n";
                    }
} while (option != 4);

                break;
            }

            case 2: {
                int option;

                do {
                    cout << "\n========== ADMIN PORTAL ==========\n";
                    cout << "1. Clubs Management\n";
                    cout << "2. Faculty Management\n";
                    cout << "3. Back\n";

                    cout << "\nEnter your choice: ";
                    cin >> option;

                    switch (option) {
                        case 1:
                            adminClubMenu();
                            break;

                        case 2:
                            adminFacultyMenu();
                            break;

                        case 3:
                            break;
default:
                            cout << "\nInvalid choice.\n";
                    }

                } while (option != 3);

                break;
            }

            case 3:
                cout << "\nThank you for using UniVerse!\n";
                break;

            default:
                cout << "\nInvalid choice.\n";
        }

    } while (choice != 3);

    return 0;
}

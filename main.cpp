#include <iostream>
#include <cstdlib>
#include "FileManager.h"
#include "Admin.h"
#include "User.h"
#include "Logger.h"
using namespace std;

int main() {
    try {
        FileManager fm;
        fm.loadAll();
        cout << "Welcome to the Movie Ticket Reservation System\n";

        int mainChoice = -1;
        while (mainChoice != 0) {
            cout << "\nMain Menu:\n1. Admin\n2. User\n0. Exit\nChoice: ";
            cin >> mainChoice; cin.ignore();
            if (mainChoice == 1) {
                string pwd;
                cout << "Enter admin password: "; getline(cin, pwd);
                Admin admin(&fm);
                if (admin.login(pwd)) admin.menu();
                else cout << "Invalid password.\n";
            } else if (mainChoice == 2) {
                int uid; string uname;
                cout << "Enter user id (int): "; cin >> uid; cin.ignore();
                cout << "Enter your name: "; getline(cin, uname);
                User user(uid, uname, &fm);
                user.menu();
            } else if (mainChoice == 0) {
                cout << "Exiting, saving data...\n";
                fm.saveAll();
                cout << "Saved. Goodbye.\n";
            } else {
                cout << "Invalid choice.\n";
            }
        }
        return 0;
    } catch (const std::exception &e) {
        // log and exit
        logException(e);
        cerr << "Fatal error: " << e.what() << "\nExiting.\n";
        return EXIT_FAILURE;
    } catch (...) {
        logActivity("Fatal unknown error (non-standard exception)");
        cerr << "Fatal unknown error. Exiting.\n";
        return EXIT_FAILURE;
    }
}

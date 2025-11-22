#include "Admin.h"
#include <iostream>
#include "Report.h"
#include "IDGenerator.h"
#include "Exceptions.h"
using namespace std;

Admin::Admin(FileManager *fm) : Person(0, "Admin"), password_("admin123"), fm_(fm) {}

bool Admin::login(const string &pwd) const { return pwd == password_; }

void Admin::menu() {
    int choice = -1;
    while (choice != 0) {
        cout << "\n--- ADMIN MENU ---\n";
        cout << "1. Add Movie\n2. Add Show\n3. View Movies\n4. View Shows\n5. Reports (Revenue & Seats)\n6. Remove Movie\n7. Remove Show\n0. Logout\nChoice: ";
        cin >> choice; cin.ignore();
        try {
            if (choice == 1) {
                int id = generateMovieId();
                string name, genre;
                cout << "Enter movie name: "; getline(cin, name);
                cout << "Enter movie genre: "; getline(cin, genre);
                Movie m(id, name, genre);
                if (fm_->addMovie(m)) cout << "Movie added with ID " << id << "\n";
                else cout << "Failed to add movie (limit reached)\n";
            } else if (choice == 2) {
                int mid;
                string time;
                int rows, cols;
                cout << "Enter Movie ID for this show: "; cin >> mid; cin.ignore();
                if (fm_->findMovieIndexById(mid) == -1) { cout << "Movie not found.\n"; continue; }
                cout << "Enter show time (e.g., 18:30 or 2025-11-05 18:30): "; getline(cin, time);
                cout << "Enter rows and cols (max 10 each): "; cin >> rows >> cols; cin.ignore();
                int sid = generateShowId();
                Show s(sid, mid, time, rows, cols);
                if (fm_->addShow(s)) cout << "Show added with ID " << sid << "\n";
                else cout << "Failed to add show.\n";
            } else if (choice == 3) {
                fm_->listMovies();
            } else if (choice == 4) {
                fm_->listShows();
            } else if (choice == 5) {
                // use polymorphic report objects
                ReportBase *r1 = new RevenueReport();
                ReportBase *r2 = new SeatReport();
                r1->generate(*fm_);
                r2->generate(*fm_);
                delete r1; delete r2;
            } else if (choice == 6) {
                int mid;
                cout << "Enter Movie ID to remove: "; cin >> mid; cin.ignore();
                double refund = 0.0;
                if (!fm_->removeMovieById(mid, refund)) {
                    cout << "Movie not found.\n";
                } else {
                    cout << "Movie with ID " << mid << " removed.";
                    if (refund > 0.0) cout << " Refunded Rs. " << refund;
                    cout << "\n";
                }
            } else if (choice == 7) {
                int sid;
                cout << "Enter Show ID to remove: "; cin >> sid; cin.ignore();
                int sidx = fm_->findShowIndexById(sid);
                if (sidx == -1) {
                    cout << "Show not found.\n";
                } else {
                    for (int i = sidx; i < fm_->showCount - 1; ++i) {
                        fm_->shows_[i] = fm_->shows_[i + 1];
                    }
                    fm_->showCount--;
                    cout << "Show with ID " << sid << " removed.\n";
                }
            } else if (choice == 0) {
                cout << "Logging out admin...\n";
            } else {
                cout << "Invalid choice.\n";
            }
        } catch (const NotFoundException &e) {
            cout << "Error: " << e.what() << "\n";
        } catch (const exception &e) {
            cout << "An error occurred: " << e.what() << "\n";
        }
    }
}

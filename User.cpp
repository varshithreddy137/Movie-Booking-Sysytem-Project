#include "User.h"
#include <iostream>
#include "Payment.h"
#include "Logger.h"
#include <sstream>
#include <vector>
#include "IDGenerator.h"
#include "Exceptions.h"
using namespace std;

User::User(int uid, const string &name, FileManager *fm) : Person(uid, name), fm_(fm) {}

void User::menu() {
    int choice = -1;
    while (choice != 0) {
        cout << "\n--- USER MENU (" << name_ << ") ---\n";
        cout << "1. View Movies\n2. View Shows\n3. Search Movie by name\n4. Book Ticket\n5. Cancel Booking\n6. View My Bookings\n0. Logout\nChoice: ";
        cin >> choice; cin.ignore();
        try {
        if (choice == 1) {
            fm_->listMovies();
        } else if (choice == 2) {
            fm_->listShows();
        } else if (choice == 3) {
            string key;
            cout << "Enter search keyword (case-sensitive substring match): ";
            getline(cin, key);
            fm_->searchMoviesByName(key);
        } else if (choice == 4) {
            // show available shows with movie name, time and availability before asking
            fm_->listShows();
            int sid;
            cout << "Enter Show ID you want to book: "; cin >> sid; cin.ignore();
            int sidx = fm_->findShowIndexById(sid);
            if (sidx == -1) { cout << "Show not found.\n"; continue; }
            Show &s = fm_->shows_[sidx];
            s.displayLayout();
            int n;
            cout << "How many tickets do you want to book? "; cin >> n; cin.ignore();
            if (n <= 0) { cout << "Invalid number of tickets.\n"; continue; }
            vector<pair<int,int>> seats;
            for (int i = 0; i < n; ++i) {
                int r,c;
                cout << "Enter seat " << (i+1) << " (row col): "; cin >> r >> c; cin.ignore();
                if (!s.isValidSeat(r,c)) { cout << "Invalid seat. Pick again.\n"; --i; continue; }
                if (!s.isSeatAvailable(r,c)) { cout << "Seat already booked. Pick another.\n"; --i; continue; }
                seats.emplace_back(r,c);
            }

            double total = 0.0;
            for (auto &p : seats) total += s.seatPrice(p.first, p.second);

           if (!Payment::process(total)) {
                cout << "Payment failed.\n";
                continue;
            }


            int bid = generateBookingId();
            // build seat list string like "(r1,c1);(r2,c2)"
            string seatList;
            for (size_t i = 0; i < seats.size(); ++i) {
                stringstream ss; ss << "(" << seats[i].first << "," << seats[i].second << ")";
                if (i) seatList += ";";
                seatList += ss.str();
            }

            bool storedAny = false;
            for (auto &p : seats) {
                int r = p.first, c = p.second;
                if (!s.bookSeat(r,c)) {
                    cout << "Booking failed for seat (" << r << "," << c << ")\n";
                    continue;
                }
                double price = s.seatPrice(r,c);
                Booking b(bid, id_, sid, r, c, price, name_);
                if (fm_->addBooking(b)) {
                    fm_->appendUserLog(id_, b);
                    storedAny = true;
                }
            }

            if (storedAny) {
                // create a temporary booking-like object for receipt (amount = total)
                Booking receiptB(bid, id_, sid, seats[0].first, seats[0].second, total, name_);
                fm_->writeReceipt(receiptB, (int)seats.size(), seatList, name_);
                cout << "Booking successful. Booking ID: " << bid << "\n";
                stringstream ss; ss << "User " << id_ << " booked " << bid << " show " << sid << " (" << seats.size() << " tickets)";
                logActivity(ss.str());
            } else {
                cout << "Could not store booking(s) (limit reached or errors)\n";
            }
        } else if (choice == 5) {
            int bid;
            cout << "Enter Booking ID to cancel: "; cin >> bid; cin.ignore();
            int idx = fm_->findBookingIndexById(bid);
            if (idx == -1) { cout << "Booking not found.\n"; continue; }
            Booking b = fm_->bookings_[idx];
            if (b.getUserId() != id_) { cout << "This booking does not belong to you.\n"; continue; }
            double refund = 0.0;
            int removed = fm_->removeBookingsByBookingId(bid, id_, refund);
            if (removed > 0) {
                cout << "Booking cancelled and " << removed << " seat(s) released. Refund Rs. " << refund << " (simulated)\n";
                stringstream ss; ss << "User " << id_ << " cancelled booking " << bid << " (removed " << removed << " seats, refund " << refund << ")"; logActivity(ss.str());
            } else {
                cout << "No bookings removed.\n";
            }
        } else if (choice == 6) {
            cout << "Your bookings:\n";
            fm_->listBookingsForUser(id_, name_);
        } else if (choice == 0) {
            cout << "Logging out user...\n";
        } else {
            cout << "Invalid choice.\n";
        }
        } catch (const exception &e) {
            cout << "An error occurred: " << e.what() << "\n";
        }
    }
}

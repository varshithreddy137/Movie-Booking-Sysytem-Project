#pragma once
#include <string>
#include <fstream>
using namespace std;

class Booking {
private:
    int bookingId_;
    int userId_;
    int showId_;
    int row_;
    int col_;
    double amount_;
    string userName_;
public:
    Booking();
    Booking(int bid, int uid, int sid, int r, int c, double amt, const string &userName = "");
    int getBookingId() const;
    int getUserId() const;
    string getUserName() const;
    int getShowId() const;
    int getRow() const;
    int getCol() const;
    double getAmount() const;
    void saveToStream(ofstream &ofs) const;
    void loadFromString(const string &line);
    void display() const;
};

#include "Booking.h"
#include <sstream>
#include <iostream>
#include <iomanip>
#include <vector>
#include "Logger.h"
using namespace std;

Booking::Booking() : bookingId_(0), userId_(0), showId_(0), row_(0), col_(0), amount_(0.0) {}
Booking::Booking(int bid, int uid, int sid, int r, int c, double amt, const string &userName)
    : bookingId_(bid), userId_(uid), showId_(sid), row_(r), col_(c), amount_(amt), userName_(userName) {}
int Booking::getBookingId() const { return bookingId_; }
int Booking::getUserId() const { return userId_; }
string Booking::getUserName() const { return userName_; }
int Booking::getShowId() const { return showId_; }
int Booking::getRow() const { return row_; }
int Booking::getCol() const { return col_; }
double Booking::getAmount() const { return amount_; }

void Booking::saveToStream(ofstream &ofs) const {
    // write as CSV: bookingId,userId,"userName",showId,row,col,amount
    ofs << bookingId_ << "," << userId_ << "," << '"' << userName_ << '"' << ","
        << showId_ << "," << row_ << "," << col_ << "," << amount_ << "\n";
}

void Booking::loadFromString(const string &line) {
    // parse CSV line supporting optional quoted userName field.
    auto parseCsv = [](const string &s) {
        vector<string> out; string cur; bool inQuotes = false;
        for (size_t i = 0; i < s.size(); ++i) {
            char ch = s[i];
            if (ch == '"') { inQuotes = !inQuotes; continue; }
            if (ch == ',' && !inQuotes) { out.push_back(cur); cur.clear(); }
            else cur.push_back(ch);
        }
        out.push_back(cur);
        return out;
    };
    vector<string> toks = parseCsv(line);
    try {
        if (toks.size() == 6) {
            // Old format: bookingId,userId,showId,row,col,amount
            bookingId_ = stoi(toks[0]);
            userId_ = stoi(toks[1]);
            showId_ = stoi(toks[2]);
            row_ = stoi(toks[3]);
            col_ = stoi(toks[4]);
            amount_ = stod(toks[5]);
            userName_.clear();
        } else if (toks.size() >= 7) {
            // New format: bookingId,userId,userName,showId,row,col,amount
            bookingId_ = stoi(toks[0]);
            userId_ = stoi(toks[1]);
            userName_ = toks[2];
            showId_ = stoi(toks[3]);
            row_ = stoi(toks[4]);
            col_ = stoi(toks[5]);
            amount_ = stod(toks[6]);
        } else {
            throw runtime_error("Malformed booking line: insufficient fields");
        }
    } catch (const exception &e) {
        stringstream ss; ss << "Failed to parse booking line: '" << line << "' : " << e.what();
        logActivity(ss.str());
        throw; // rethrow to let caller handle
    }
}

void Booking::display() const {
    cout << "Booking ID: " << bookingId_ << " | ShowID: " << showId_
         << " | Seat: (" << row_ << "," << col_ << ")"
         << " | Amount: " << fixed << setprecision(2) << amount_;
    if (!userName_.empty()) cout << " | User: " << userName_;
    cout << "\n";
}

#include "Show.h"
#include <iostream>
#include <iomanip>
using namespace std;

Show::Show() : showId_(0), movieId_(0), time_(), rows_(0), cols_(0) {}

Show::Show(int sid, int mid, const string &time, int rows, int cols)
    : showId_(sid), movieId_(mid), time_(time),
      rows_(rows > 10 ? 10 : rows), cols_(cols > 10 ? 10 : cols) {
    for (int i = 0; i < rows_; ++i)
        for (int j = 0; j < cols_; ++j)
            seats_[i][j] = (i < (rows_ * 0.4)) ? Seat(PREMIUM) : Seat(REGULAR);
}

int Show::getShowId() const { return showId_; }
int Show::getMovieId() const { return movieId_; }
string Show::getTime() const { return time_; }
int Show::getRows() const { return rows_; }
int Show::getCols() const { return cols_; }

bool Show::isValidSeat(int r, int c) const {
    return r >= 1 && r <= rows_ && c >= 1 && c <= cols_;
}
bool Show::isSeatAvailable(int r, int c) const {
    if (!isValidSeat(r,c)) return false;
    return !seats_[r-1][c-1].isBooked();
}
bool Show::bookSeat(int r, int c) {
    if (!isValidSeat(r,c)) return false;
    if (seats_[r-1][c-1].isBooked()) return false;
    seats_[r-1][c-1].book();
    return true;
}
bool Show::cancelSeat(int r, int c) {
    if (!isValidSeat(r,c)) return false;
    if (!seats_[r-1][c-1].isBooked()) return false;
    seats_[r-1][c-1].cancel();
    return true;
}
double Show::seatPrice(int r, int c) const {
    if (!isValidSeat(r,c)) return 0.0;
    return seats_[r-1][c-1].price();
}
int Show::availableCount() const {
    int cnt = 0;
    for (int i=0;i<rows_;++i)
        for (int j=0;j<cols_;++j)
            if (!seats_[i][j].isBooked()) ++cnt;
    return cnt;
}
int Show::totalCount() const { return rows_ * cols_; }

void Show::displayLayout() const {
    cout << "\nShow ID: " << showId_ << " Time: " << time_ << " (Rows:" << rows_ << " Cols:" << cols_ << ")\n";
    cout << "Legend: P=Premium available, R=Regular available, X=Booked\n";
    for (int i=0;i<rows_;++i) {
        cout << "Row " << (i+1) << ": ";
        for (int j=0;j<cols_;++j) cout << seats_[i][j].symbol() << " ";
        cout << "\n";
    }
}

void Show::saveToStream(ofstream &ofs) const {
    ofs << showId_ << "|" << movieId_ << "|" << time_ << "|" << rows_ << "|" << cols_ << "\n";
    for (int i=0;i<rows_;++i) {
        for (int j=0;j<cols_;++j) {
            int b = seats_[i][j].isBooked() ? 1 : 0;
            int t = (seats_[i][j].getType() == PREMIUM) ? 1 : 0;
            ofs << b << t;
            if (j < cols_-1) ofs << " ";
        }
        ofs << "\n";
    }
}

void Show::loadFromStream(ifstream &ifs) {
    string header;
    if (!getline(ifs, header)) return;
    if (header.empty()) return;
    size_t p1 = header.find('|');
    size_t p2 = header.find('|', p1+1);
    size_t p3 = header.find('|', p2+1);
    size_t p4 = header.find('|', p3+1);
    showId_ = stoi(header.substr(0,p1));
    movieId_ = stoi(header.substr(p1+1,p2-p1-1));
    time_ = header.substr(p2+1,p3-p2-1);
    rows_ = stoi(header.substr(p3+1,p4-p3-1));
    cols_ = stoi(header.substr(p4+1));
    for (int i=0;i<rows_;++i) {
        string line;
        getline(ifs, line);
        int colIdx = 0;
        for (size_t pos=0; pos<line.size();) {
            if (line[pos] == ' ') { ++pos; continue; }
            if (pos+1 >= line.size()) break;
            int b = line[pos]-'0';
            int t = line[pos+1]-'0';
            seats_[i][colIdx] = (t==1) ? Seat(PREMIUM) : Seat(REGULAR);
            if (b==1) seats_[i][colIdx].book();
            ++colIdx;
            pos += 2;
            if (pos < line.size() && line[pos] == ' ') ++pos;
        }
    }
}

void Show::displaySummary() const {
    cout << "ShowID:" << showId_ << " MovieID:" << movieId_ << " Time:" << time_
         << " Available:" << availableCount() << "/" << totalCount() << "\n";
}
